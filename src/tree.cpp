#include "tree.h"
#include <sstream>
// -------------------------------------------------------------
// ------------------------ 变量生成方法 -------------------------
// -------------------------------------------------------------

// 1.全局变量：由program节点调用gen_var_decl函数进行声明
// 2.局部变量：所有对于局部变量的引用使用%l+作用域+变量名进行生成和使用
// 3.临时变量：标号由每个Function进行管理，由Function类中的Ask_tmp_var方法进行生成和管理
// 4.变量传递：使用栈gen_var_label_stack在节点间传递变量标号

// --------------------------------------------------------------
// --------------------- 工具函数 重载运算符 ----------------------
// --------------------------------------------------------------

string operator+(string &content, int number)
{
    return content + to_string(number);
}

string &operator+=(string &content, int number)
{
    return content = content + to_string(number);
}

// ------------------------------------------------------------
// ----------------------- 全局变量 ----------------------------
// ------------------------------------------------------------

// 字符流
stringstream sstream;
stringstream finalstream;
stringstream var_declare_stream;

// multimap <标识符名称， 作用域> 变量名列表
multimap<string, string> idNameList;
// map <<标识符名称， 作用域>, 结点指针> 变量列表
map<pair<string, string>, TreeNode *> idList;

// map <函数名， 返回类型> 函数名列表
map<string, Type *> FuncList;
// stack <函数类> 当前所在函数对应的函数类，临时变量标号等以函数为单位生成
stack<Function *> func_stack;
/**
 * 生成的临时标号管理栈，在子节点push，在父节点pop，每次应当只处理一个临时变量
 * 在push时应当push形如%[gtl][name]的字符串
 */
stack<string> gen_var_label_stack;
// map <作用域+变量名， 节点指针> 局部变量表，在每次函数定义前清空
map<string, TreeNode *> LocalVarList;

// 当前所处函数的声明结点指针，return使用
TreeNode *pFunction;

// 循环体栈，为continue与break配对使用
TreeNode *cycleStack[10];
int cycleStackTop = -1;

// ------------------------------------------------------------
// ------------------------ 代码 ------------------------------
// ------------------------------------------------------------

TreeNode::TreeNode(int lineno, NodeType type)
{
    this->lineno = lineno;
    this->nodeType = type;
    this->pointLevel = 0;
}

TreeNode::TreeNode(TreeNode *node)
{
    this->lineno = node->lineno;
    this->nodeType = node->nodeType;
    this->optype = node->optype;
    this->stype = node->stype;
    this->type = node->type;
    this->int_val = node->int_val;
    this->var_name = node->var_name;
    this->var_scope = node->var_scope;
    this->pointLevel = node->pointLevel;
}

void TreeNode::addChild(TreeNode *child)
{
    if (this->child == nullptr)
    {
        this->child = child;
    }
    else
    {
        this->child->addSibling(child);
    }
}

void TreeNode::addSibling(TreeNode *sibling)
{
    TreeNode *p = this;
    while (p->sibling != nullptr)
    {
        p = p->sibling;
    }
    p->sibling = sibling;
}

int TreeNode::getChildNum()
{
    int num = 0;
    for (TreeNode *p = child; p != nullptr; p = p->sibling)
        num++;
    return num;
}

int TreeNode::getVal()
{
    if (nodeType == NODE_CONST)
    {
        switch (type->type)
        {
        case VALUE_INT:
            return int_val;
        default:
            return 0;
        }
    }
    else if (child->nodeType == NODE_CONST)
    {
        return child->getVal();
    }
    return 0;
}

void TreeNode::genNodeId()
{
    static unsigned int maxid = 0;
    this->nodeID = maxid++;
    if (this->child)
        this->child->genNodeId();
    if (this->sibling)
        this->sibling->genNodeId();
}

void TreeNode::genCode()
{
    TreeNode *p = this->child; // p = this->child
    TreeNode **q;
    int N = 0, n = 1, pSize = 0;
    string varCode = "";
    switch (nodeType)
    {
    case NODE_PROG:
        gen_var_decl(); //生成全局变量声明
        while (p)       //循环生成子语句IR
        {
            if (p->nodeType == NODE_STMT && p->stype == STMT_FUNCDECL)
                p->genCode();
            p = p->sibling;
        }
        break;
    case NODE_FUNCALL:
    { // 生成参数
        N = p->sibling->getChildNum();
        p = p->sibling->child;
#ifdef childNumdebug
        sstream << "# ChildNum = " << N << endl;
#endif
        vector<string> var_list;
        for (int i = 0; i < N; i++)
        {
            p->genCode();
            p = p->sibling;
            var_list.push_back(gen_var_label_stack.top());
            gen_var_label_stack.pop();
            pSize += this->child->type->paramType[i]->getSize();
        }
        auto it = FuncList.find(child->var_name);
        if (it->second->getTypeInfo() == "i32")
        {
            string tmp_var_label = func_stack.top()->Ask_tmp_var();
            sstream << "\t" << tmp_var_label << " = ";
            gen_var_label_stack.push(tmp_var_label);
        }
        else
            sstream << "\t";
        sstream << "call " << it->second->getTypeInfo() << " @" << it->first;
        if (N > 0)
        {
            sstream << "(";
            for (int i = 0; i < N; i++)
            {
                if (i != 0)
                    sstream << ", ";
                sstream << var_list[i];
            }
            sstream << ")";
        }
        sstream << endl;
    }
    break;
    case NODE_STMT:
        switch (stype)
        {
        case STMT_FUNCDECL:
        {
            cycleStackTop = -1;
            pFunction = this;
            auto type = this->child->type;
            auto func_new = new Function(this);
            func_new->return_var = type->type == VALUE_INT ? "%t" + this->child->sibling->var_name + "return" : "";
            func_stack.push(func_new);
            // 生成返回标签
            this->get_label();
            // 生成局部变量定义表
            this->gen_var_decl();
            // 确定返回值类型
            string print_return_type = type->type == VALUE_INT ? "i32 @" : "void @";
            string return_var_declare = func_new->return_var == "" ? "" : "\tdeclare i32 " + func_new->return_var + "\n";
            var_declare_stream << return_var_declare;
            // 生成函数声明头
            finalstream << "declare " << print_return_type << this->child->sibling->var_name;
            // 生成参数列表
            finalstream << "(";
            if (!LocalVarList.empty())
            {
                bool start = false;
                for (auto iter_def_var = LocalVarList.begin(); iter_def_var != LocalVarList.end(); iter_def_var++)
                {
                    if (iter_def_var->second->nodeType == NODE_PARAM)
                    {
                        if (start)
                            sstream << ", ";
                        string labels = iter_def_var->first;
                        sstream << iter_def_var->second->child->sibling->type->getTypeInfo() << " %t"
                                << "tmp" << labels;
                        start = true;
                    }
                }
            }
            finalstream << "){" << endl;
            // 生成参数declare语句
            for (auto iter_def_var = LocalVarList.begin(); iter_def_var != LocalVarList.end(); iter_def_var++)
            {
                if (iter_def_var->second->nodeType != NODE_PARAM)
                {
                    var_declare_stream << "\tdeclare " << iter_def_var->second->type->getTypeInfo();
                    var_declare_stream << " %l" << iter_def_var->first;
                    if (iter_def_var->second->type->dim != 0)
                    {
                        for (unsigned int i = 0; i < iter_def_var->second->type->dim; i++)
                            var_declare_stream << "[" << iter_def_var->second->type->dimSize[i] << "]";
                    }
                    var_declare_stream << endl;
                }
            }
            sstream << "\tentry" << endl;
            // 生成函数参数初始化语句
            if (!LocalVarList.empty())
            {
                for (auto iter_def_var = LocalVarList.begin(); iter_def_var != LocalVarList.end(); iter_def_var++)
                {
                    if (iter_def_var->second->nodeType == NODE_PARAM)
                    {
                        sstream << "\t%l" << iter_def_var->first << " = "
                                << "%ttmp" << iter_def_var->first << endl;
                    }
                }
            }
            // 内部代码递归生成
            p->sibling->sibling->sibling->genCode();
            // 产生返回标签代码
            sstream << this->label.next_label << ":" << endl;
            sstream << "\texit " << func_stack.top()->return_var << endl;
            sstream << "}" << endl;
            func_stack.pop();
            pFunction = nullptr;
            string s_out;
            while (getline(var_declare_stream, s_out))
            {
                finalstream << s_out << endl;
            }
            var_declare_stream.clear();
            while (getline(sstream, s_out))
            {
                finalstream << s_out << endl;
            }
            sstream.clear();
        }
        break;
        case STMT_DECL:
            // p = p->sibling->child;
            // while (p)
            // {
            //     if (p->nodeType == NODE_OP)
            //     {
            //         p->child->sibling->genCode();
            //         // 这里也很蠢，可以通过三地址码优化一下
            //         sstream << "\tmovl\t%eax, " << LocalVarList[p->child->var_scope + p->child->var_name] << "(%ebp)" << endl;
            //     }
            //     p = p->sibling;
            // }
            break;
        case STMT_IF:
        {
            get_label();
            sstream << label.begin_label << ":" << endl;
            this->child->genCode();
            if (this->child->nodeType == NODE_OP && (this->child->optype != OP_AND && this->child->optype != OP_OR && this->child->optype != OP_NOT))
            {
                string a = gen_var_label_stack.top();
                gen_var_label_stack.pop();
                sstream << "\tbc " << a << ", " << label.true_label << ", " << label.false_label << endl;
            }
            sstream << label.true_label << ":" << endl;
            this->child->sibling->genCode();
            sstream << label.false_label << ":" << endl;
        }
        break;
        case STMT_IFELSE:
        {
            get_label();
            sstream << label.begin_label << ":" << endl;
            this->child->genCode();
            if (this->child->nodeType == NODE_OP && (this->child->optype != OP_AND && this->child->optype != OP_OR && this->child->optype != OP_NOT))
            {
                string a = gen_var_label_stack.top();
                gen_var_label_stack.pop();
                sstream << "\tbc " << a << ", " << label.true_label << ", " << label.false_label << endl;
            }
            sstream << label.true_label << ":" << endl;
            this->child->sibling->genCode();
            sstream << "\tbr " << label.next_label << endl;
            sstream << label.false_label << ":" << endl;
            this->child->sibling->sibling->genCode();
            sstream << label.next_label << ":" << endl;
        }
        break;
        case STMT_WHILE:
        {
            get_label();
            cycleStack[++cycleStackTop] = this;
            sstream << label.next_label << ":" << endl;
            this->child->genCode();
            if (this->child->nodeType == NODE_OP && (this->child->optype != OP_AND && this->child->optype != OP_OR && this->child->optype != OP_NOT))
            {
                string a = gen_var_label_stack.top();
                gen_var_label_stack.pop();
                sstream << "\tbc " << a << ", " << label.true_label << ", " << label.false_label << endl;
            }
            sstream << label.true_label << ":" << endl;
            this->child->sibling->genCode();
            sstream << "\tbr " << label.next_label << endl;
            sstream << label.false_label << ":" << endl;
            cycleStackTop--;
        }
        break;
        case STMT_BREAK:
            sstream << "\tbr " << cycleStack[cycleStackTop]->label.false_label << endl;
            break;
        case STMT_CONTINUE:
            sstream << "\tbr " << cycleStack[cycleStackTop]->label.next_label << endl;
            break;
        case STMT_RETURN:
            if (p)
            {
                p->genCode();
                string a = gen_var_label_stack.top();
                gen_var_label_stack.pop();
                sstream << "\t" << func_stack.top()->return_var << " = " << a << endl;
            }
            sstream << "\tbr " << pFunction->label.next_label << endl;
            break;
        case STMT_BLOCK:
            while (p)
            {
                p->genCode();
                p = p->sibling;
            }
            break;
        default:
            break;
        }
        break;
    case NODE_EXPR:
        if (child->nodeType == NODE_VAR)
        {
            // 内存变量（全局/局部）
            string varCode = getVarNameCode(this->child);
            if (child->pointLevel == 0)
                gen_var_label_stack.push(varCode);
            // else if (child->pointLevel < 0)
            // { // &前缀的变量
            //     sstream << "\tleal\t" << varCode << ", %eax" << endl;
            // }
            // else
            // {
            //     sstream << "\tmovl\t" << varCode << ", %eax" << endl;
            //     for (int i = 0; i < child->pointLevel; i++)
            //     {
            //         sstream << "\tmovl\t(%eax), %eax" << endl;
            //     }
            // }
        }
        // else if (child->nodeType == NODE_OP && child->optype == OP_INDEX)
        // {
        //     // 数组
        //     child->genCode();
        // }
        else
        {
            gen_var_label_stack.push(to_string(child->getVal()));
        }
        break;
    case NODE_OP:
        switch (optype)
        {
        case OP_EQ:
        {
            p->genCode();
            string a = gen_var_label_stack.top();
            gen_var_label_stack.pop();
            p->sibling->genCode();
            string b = gen_var_label_stack.top();
            gen_var_label_stack.pop();
            string tmp_var_label = func_stack.top()->Ask_tmp_var();
            sstream << "\t" << tmp_var_label << " = eq " << a << ", " << b << endl;
            gen_var_label_stack.push(tmp_var_label);
        }
        break;
        case OP_NEQ:
        {
            p->genCode();
            string a = gen_var_label_stack.top();
            gen_var_label_stack.pop();
            p->sibling->genCode();
            string b = gen_var_label_stack.top();
            gen_var_label_stack.pop();
            string tmp_var_label = func_stack.top()->Ask_tmp_var();
            sstream << "\t" << tmp_var_label << " = ne " << a << ", " << b << endl;
            gen_var_label_stack.push(tmp_var_label);
        }
        break;
        case OP_GRA:
        {
            p->genCode();
            string a = gen_var_label_stack.top();
            gen_var_label_stack.pop();
            p->sibling->genCode();
            string b = gen_var_label_stack.top();
            gen_var_label_stack.pop();
            string tmp_var_label = func_stack.top()->Ask_tmp_var();
            sstream << "\t" << tmp_var_label << " = gt " << a << ", " << b << endl;
            gen_var_label_stack.push(tmp_var_label);
        }
        break;
        case OP_LES:
        {
            p->genCode();
            string a = gen_var_label_stack.top();
            gen_var_label_stack.pop();
            p->sibling->genCode();
            string b = gen_var_label_stack.top();
            gen_var_label_stack.pop();
            string tmp_var_label = func_stack.top()->Ask_tmp_var();
            sstream << "\t" << tmp_var_label << " = lt " << a << ", " << b << endl;
            gen_var_label_stack.push(tmp_var_label);
        }
        break;
        case OP_GRAEQ:
        {
            p->genCode();
            string a = gen_var_label_stack.top();
            gen_var_label_stack.pop();
            p->sibling->genCode();
            string b = gen_var_label_stack.top();
            gen_var_label_stack.pop();
            string tmp_var_label = func_stack.top()->Ask_tmp_var();
            sstream << "\t" << tmp_var_label << " = ge " << a << ", " << b << endl;
            gen_var_label_stack.push(tmp_var_label);
        }
        break;
        case OP_LESEQ:
        {
            p->genCode();
            string a = gen_var_label_stack.top();
            gen_var_label_stack.pop();
            p->sibling->genCode();
            string b = gen_var_label_stack.top();
            gen_var_label_stack.pop();
            string tmp_var_label = func_stack.top()->Ask_tmp_var();
            sstream << "\t" << tmp_var_label << " = le " << a << ", " << b << endl;
            gen_var_label_stack.push(tmp_var_label);
        }
        break;
        case OP_NOT:
            get_label();
            p->genCode();
            // sstream << "\tandl\t%eax, $eax" << endl
            //      << "\tsete\t%al" << endl;
            break;
        case OP_AND:
        {
            get_label();
            // 对于表达式1进行判定
            p->genCode();
            // 当子表达式为逻辑表达式时，跳转指令由子表达式生成
            if (p->nodeType == NODE_OP &&
                (p->optype == OP_AND || p->optype == OP_OR || p->optype == OP_NOT))
            {
            }
            else
            {
                string a = gen_var_label_stack.top();
                gen_var_label_stack.pop();
                sstream << "\tbc " << a << ", " << this->child->label.true_label << ", " << this->child->label.false_label << endl;
            }
            sstream << this->child->label.false_label << ":" << endl;
            // 对于表达式2进行判定
            p->sibling->genCode();
            // 当子表达式为逻辑表达式时，跳转指令由子表达式生成
            if (p->sibling->nodeType == NODE_OP &&
                (p->sibling->optype == OP_AND || p->sibling->optype == OP_OR || p->sibling->optype == OP_NOT))
            {
            }
            else
            {
                string b = gen_var_label_stack.top();
                gen_var_label_stack.pop();
                sstream << "\tbc " << b << ", " << this->child->sibling->label.true_label << ", " << this->child->sibling->label.false_label << endl;
            }
        }
        break;
        case OP_OR:
        {
            get_label();
            // 对于表达式1进行判定
            p->genCode();
            // 当子表达式为逻辑表达式时，跳转指令由子表达式生成
            if (p->nodeType == NODE_OP &&
                (p->optype == OP_AND || p->optype == OP_OR || p->optype == OP_NOT))
            {
            }
            else
            {
                string a = gen_var_label_stack.top();
                gen_var_label_stack.pop();
                sstream << "\tbc " << a << ", " << this->child->label.true_label << ", " << this->child->label.false_label << endl;
            }
            sstream << this->child->label.false_label << ":" << endl;
            // 对于表达式2进行判定
            p->sibling->genCode();
            // 当子表达式为逻辑表达式时，跳转指令由子表达式生成
            if (p->sibling->nodeType == NODE_OP &&
                (p->sibling->optype == OP_AND || p->sibling->optype == OP_OR || p->sibling->optype == OP_NOT))
            {
            }
            else
            {
                string b = gen_var_label_stack.top();
                gen_var_label_stack.pop();
                sstream << "\tbc " << b << ", " << this->child->sibling->label.true_label << ", " << this->child->sibling->label.false_label << endl;
            }
        }
        break;
        case OP_ADDASSIGN:
        {
            string var_name = getVarNameCode(p);
            p->sibling->genCode();
            string target = gen_var_label_stack.top();
            gen_var_label_stack.pop();
            sstream << "\t" << var_name << " = add " << var_name << ", " << target << endl;
        }
        break;
        case OP_SUBASSIGN:
        {
            string var_name = getVarNameCode(p);
            p->sibling->genCode();
            string target = gen_var_label_stack.top();
            gen_var_label_stack.pop();
            sstream << "\t" << var_name << " = sub " << var_name << ", " << target << endl;
        }
        break;
        case OP_MULASSIGN:
        {
            string var_name = getVarNameCode(p);
            p->sibling->genCode();
            string target = gen_var_label_stack.top();
            gen_var_label_stack.pop();
            sstream << "\t" << var_name << " = mul " << var_name << ", " << target << endl;
        }
        break;
        case OP_DIVASSIGN:
        {
            string var_name = getVarNameCode(p);
            p->sibling->genCode();
            string target = gen_var_label_stack.top();
            gen_var_label_stack.pop();
            sstream << "\t" << var_name << " = div " << var_name << ", " << target << endl;
        }
        break;
        case OP_ASSIGN:
        {
            p->sibling->genCode();
            string a = gen_var_label_stack.top();
            gen_var_label_stack.pop();
            if (p->nodeType == NODE_VAR)
                sstream << "\t" << getVarNameCode(p) << " = " << a << endl;
            else // 左值是数组
            {
                // 计算偏移量
                p->child->sibling->genCode();
                string b = gen_var_label_stack.top();
                gen_var_label_stack.pop();
                sstream << "\t*" << b << " = " << a << endl;
            }
        }
        break;
        case OP_INC:
            varCode = getVarNameCode(p);
            sstream << "\t%l" << varCode << " = %l" << varCode << " + 1" << endl;
            break;
        case OP_DEC:
            varCode = getVarNameCode(p);
            sstream << "\t%l" << varCode << " = %l" << varCode << " - 1" << endl;
            break;
        case OP_ADD:
        {
            p->genCode();
            string a = gen_var_label_stack.top();
            gen_var_label_stack.pop();
            p->sibling->genCode();
            string b = gen_var_label_stack.top();
            gen_var_label_stack.pop();
            string tmp_var_label = func_stack.top()->Ask_tmp_var();
            sstream << "\t" << tmp_var_label << " = add " << a << ", " << b << endl;
            gen_var_label_stack.push(tmp_var_label);
        }
        break;
        case OP_SUB:
        {
            p->genCode();
            string a = gen_var_label_stack.top();
            gen_var_label_stack.pop();
            p->sibling->genCode();
            string b = gen_var_label_stack.top();
            gen_var_label_stack.pop();
            string tmp_var_label = func_stack.top()->Ask_tmp_var();
            sstream << "\t" << tmp_var_label << " = sub " << a << ", " << b << endl;
            gen_var_label_stack.push(tmp_var_label);
        }
        break;
        case OP_POS:
            p->genCode();
            break;
        case OP_NAG:
        {
            p->genCode();
            string a = gen_var_label_stack.top();
            string tmp_var_label = func_stack.top()->Ask_tmp_var();
            sstream << "\t" << tmp_var_label << " = neg " << a << endl;
            gen_var_label_stack.push(tmp_var_label);
        }
        break;
        case OP_MUL:
        {
            p->genCode();
            string a = gen_var_label_stack.top();
            gen_var_label_stack.pop();
            p->sibling->genCode();
            string b = gen_var_label_stack.top();
            gen_var_label_stack.pop();
            string tmp_var_label = func_stack.top()->Ask_tmp_var();
            sstream << "\t" << tmp_var_label << " = mul " << a << ", " << b << endl;
            gen_var_label_stack.push(tmp_var_label);
        }
        break;
        case OP_DIV:
        {
            p->genCode();
            string a = gen_var_label_stack.top();
            gen_var_label_stack.pop();
            p->sibling->genCode();
            string b = gen_var_label_stack.top();
            gen_var_label_stack.pop();
            string tmp_var_label = func_stack.top()->Ask_tmp_var();
            sstream << "\t" << tmp_var_label << " = div " << a << ", " << b << endl;
            gen_var_label_stack.push(tmp_var_label);
        }
        break;
        case OP_MOD:
        {
            p->genCode();
            string a = gen_var_label_stack.top();
            gen_var_label_stack.pop();
            p->sibling->genCode();
            string b = gen_var_label_stack.top();
            gen_var_label_stack.pop();
            string tmp_var_label = func_stack.top()->Ask_tmp_var();
            sstream << "\t" << tmp_var_label << " = mod " << a << ", " << b << endl;
            gen_var_label_stack.push(tmp_var_label);
        }
        break;
        case OP_INDEX:
            // 这里只生成下标运算在右值时的代码（即按下标取数值）
            {
                p->sibling->genCode();
                string a = gen_var_label_stack.top();
                gen_var_label_stack.pop();
                string tmp_var_label = func_stack.top()->Ask_tmp_var();
                sstream << "\t" << tmp_var_label << " = add " << getVarNameCode(this) << ", " << a << endl;
                gen_var_label_stack.push(tmp_var_label);
            }
            break;
        default:
            break;
        }
    default:
        break;
    }
}

void TreeNode::gen_var_decl()
{
    if (nodeType == NODE_PROG)
    {
        // 根节点下只处理全局变量声明
        TreeNode *p = child;
        bool print_data = false;
        while (p)
        {
            // 发现了p为定义语句，LeftChild为类型，RightChild为声明表
            if (p->stype == STMT_DECL || p->stype == STMT_CONSTDECL)
            {
                TreeNode *q = p->child->sibling->child;
                // q为变量表语句，可能为标识符或者赋值声明运算符
                while (q)
                {
                    TreeNode *t = q;
                    finalstream << "declare i32 @g1" << t->var_name;
                    if (t->type->dim != 0)
                    {
                        for (unsigned int i = 0; i < t->type->dim; i++)
                            finalstream << "[" << t->type->dimSize[i] << "]";
                    }
                    finalstream << endl;
                    q = q->sibling;
                }
            }
            p = p->sibling;
        }
    }
    else if (nodeType == NODE_STMT && stype == STMT_FUNCDECL)
    {
        // 对于函数声明语句，递归查找局部变量声明
        LocalVarList.clear();
#ifdef varDeclDebug
        sstream << "# gen_var_decl in funcDecl init" << endl;
#endif
        // 遍历参数定义列表
        TreeNode *p = this->child->sibling->sibling->child;
        while (p)
        {
            // 只能是基本数据类型，简便起见一律分配4字节
            LocalVarList[p->child->sibling->var_scope + p->child->sibling->var_name] = p;
            p = p->sibling;
        }
#ifdef varDeclDebug
        sstream << "# gen_var_decl in funcDecl param fin" << endl;
#endif
        // 遍历代码段，查找函数内声明的局部变量
        p = child->sibling->sibling->sibling->child;
        while (p)
        {
            p->gen_var_decl();
            p = p->sibling;
        }
#ifdef varDeclDebug
        sstream << "# gen_var_decl in funcDecl fin" << endl;
#endif
    }
    else if (nodeType == NODE_STMT && (stype == STMT_DECL || stype == STMT_CONSTDECL))
    {
#ifdef varDeclDebug
        sstream << "# gen_var_decl found varDecl stmt at node " << nodeID << endl;
#endif
        // 找到了局部变量定义
        TreeNode *q = child->sibling->child;
        while (q)
        {
            // 遍历常变量列表，指针类型视为4字节int
            // q为标识符或声明赋值运算符
            TreeNode *t = q;
            // 声明时赋值
            if (t->type->dim > 0)
            {
                t->type->type = VALUE_ARRAY;
            }
            LocalVarList[t->var_scope + t->var_name] = t;
            q = q->sibling;
        }
    }
    else
    {
        // 在函数定义语句块内部递归查找局部变量声明
        TreeNode *p = child;
        while (p)
        {
            p->gen_var_decl();
            p = p->sibling;
        }
    }
}

string TreeNode::new_label()
{
    static int label_seq = 0;
    string labelStr = ".L";
    labelStr += label_seq++;
    return labelStr;
}

void TreeNode::get_label()
{
    string temp;
    switch (nodeType)
    {
    case NODE_STMT:
        switch (stype)
        {
        case STMT_FUNCDECL:
            this->label.begin_label = this->child->sibling->var_name;
            // next为return和局部变量清理
            this->label.next_label = ".LRET_" + this->child->sibling->var_name;
            break;
        case STMT_IF:
            this->label.begin_label = new_label();
            this->label.true_label = new_label();
            this->label.false_label = this->label.next_label = new_label();
            this->child->label.true_label = this->label.true_label;
            this->child->label.false_label = this->label.false_label;
            break;
        case STMT_IFELSE:
            this->label.begin_label = new_label();
            this->label.true_label = new_label();
            this->label.false_label = new_label();
            this->label.next_label = new_label();
            this->child->label.true_label = this->label.true_label;
            this->child->label.false_label = this->label.false_label;
            break;
        case STMT_WHILE:
            this->label.begin_label = this->label.next_label = new_label();
            this->label.true_label = new_label();
            this->label.false_label = new_label();
            this->child->label.true_label = this->label.true_label;
            this->child->label.false_label = this->label.false_label;
            break;
        default:
            break;
        }
        break;
    case NODE_OP:
        switch (optype)
        {
        case OP_AND:
            child->label.true_label = new_label();
            child->sibling->label.true_label = label.true_label;
            child->label.false_label = child->sibling->label.false_label = label.false_label;
            break;
        case OP_OR:
            child->label.true_label = child->sibling->label.true_label = label.true_label;
            child->label.false_label = new_label();
            child->sibling->label.false_label = label.false_label;
            break;
        case OP_NOT:
            child->label.true_label = label.false_label;
            child->label.false_label = label.true_label;
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}
/**/
string TreeNode::getVarNameCode(TreeNode *p)
{
    string varCode = "";
    if (p->nodeType == NODE_VAR)
    {
        // 标识符
        if (p->var_scope == "1")
        {
            // 全局变量
            varCode = "@g" + p->var_scope + p->var_name;
        }
        else
        {
            // 局部变量（不要跨定义域访问）
            varCode = "%l" + p->var_scope + p->var_name;
        }
    }
    else
    {
        // 数组
        if (p->child->var_scope == "1")
        {
            varCode = "@g" + p->child->var_scope + p->child->var_name;
        }
        else
        {
            varCode = "%l" + p->child->var_scope + p->child->var_name;
        }
    }
    return varCode;
}

void TreeNode::printAST()
{
    printNodeInfo();
    printChildrenId();
    sstream << endl;
    TreeNode *p = this->child;
    while (p != nullptr)
    {
        p->printAST();
        p = p->sibling;
    }
}

void TreeNode::printNodeInfo()
{
    sstream << "# @" << this->nodeID << "\t";
    sstream << "line " << lineno << "\t";
    sstream << nodeType2String(this->nodeType);
    this->printSpecialInfo();
}

void TreeNode::printChildrenId()
{
    sstream << ",\tchild:";
    TreeNode *p = this->child;
    if (p == nullptr)
        sstream << "-";
    while (p)
    {
        sstream << "\t@" << p->nodeID;
        p = p->sibling;
    }
    sstream << "\t";
}

void TreeNode::printSpecialInfo()
{
    switch (this->nodeType)
    {
    case NODE_CONST:
        sstream << ", ";
        this->printConstVal();
        break;
    case NODE_VAR:
        sstream << ",\tname: ";
        if (this->type->pointLevel != 0)
        {
            // 为指针类型添加前缀(*和&)
            string prefix = this->type->pointLevel > 0 ? "*" : "&";
            for (int i = 0; i < abs(this->type->pointLevel); i++)
                sstream << prefix;
        }
        sstream << var_name << ",\tscope: ";
        for (unsigned int i = 0; i < var_scope.length(); i++)
            sstream << var_scope[i] << " ";
        break;
    case NODE_EXPR:
        break;
    case NODE_STMT:
        sstream << ", " << sType2String(this->stype) << "\t";
        if (this->stype == STMT_DECL || this->stype == STMT_CONSTDECL)
        {
            if (this->child && this->child->sibling && this->child->sibling->type)
                sstream << this->child->sibling->type->getTypeInfo() << "\t";
        }
        break;
    case NODE_TYPE:
        sstream << ", " << this->type->getTypeInfo();
        break;
    case NODE_OP:
        sstream << ", " << opType2String(this->optype) << "\t";
        break;
    default:
        break;
    }
}

string TreeNode::nodeType2String(NodeType type)
{
    switch (type)
    {
    case NODE_CONST:
        return "<const>";
    case NODE_VAR:
        return "<var>";
    case NODE_EXPR:
        return "<expression>";
    case NODE_TYPE:
        return "<type>";
    case NODE_FUNCALL:
        return "function call";
    case NODE_STMT:
        return "<statment>";
    case NODE_PROG:
        return "<program>";
    case NODE_VARLIST:
        return "<variable list>";
    case NODE_PARAM:
        return "function format parameter";
    case NODE_OP:
        return "<operation>";
    default:
        return "<?>";
    }
}

string TreeNode::sType2String(StmtType type)
{
    switch (type)
    {
    case STMT_SKIP:
        return "skip";
    case STMT_DECL:
        return "declaration";
    case STMT_CONSTDECL:
        return "const declaration";
    case STMT_FUNCDECL:
        return "function declaration";
    case STMT_BLOCK:
        return "block";
    case STMT_IF:
        return "if";
    case STMT_IFELSE:
        return "if with else";
    case STMT_WHILE:
        return "while";
    case STMT_FOR:
        return "for";
    case STMT_RETURN:
        return "return";
    case STMT_CONTINUE:
        return "continue";
    case STMT_BREAK:
        return "break";
    default:
        return "?";
    }
}

string TreeNode::opType2String(OperatorType type)
{
    switch (type)
    {
    case OP_EQ:
        return "equal";
    case OP_NEQ:
        return "not equal";
    case OP_GRAEQ:
        return "grater equal";
    case OP_LESEQ:
        return "less equal";
    case OP_ASSIGN:
        return "assign";
    case OP_ADDASSIGN:
        return "add assign";
    case OP_SUBASSIGN:
        return "sub assign";
    case OP_MULASSIGN:
        return "multiply assign";
    case OP_DIVASSIGN:
        return "divide assign";
    case OP_GRA:
        return "grater";
    case OP_LES:
        return "less";
    case OP_INC:
        return "auto increment";
    case OP_DEC:
        return "auto decrement";
    case OP_ADD:
        return "add";
    case OP_SUB:
        return "sub";
    case OP_POS:
        return "positive";
    case OP_NAG:
        return "nagative";
    case OP_MUL:
        return "multiply";
    case OP_DIV:
        return "divide";
    case OP_MOD:
        return "Modulo";
    case OP_NOT:
        return "not";
    case OP_AND:
        return "and";
    case OP_OR:
        return "or";
    case OP_INDEX:
        return "index";
    default:
        return "?";
    }
}

void TreeNode::printConstVal()
{
    if (this->nodeType == NODE_CONST)
    {
        sstream << this->type->getTypeInfo() << ":";
        switch (this->type->type)
        {
        case VALUE_INT:
            sstream << int_val;
            break;
        default:
            sstream << "-";
            break;
        }
    }
}
// 直接生成%t[name]格式的临时变量字符串
string Function::Ask_tmp_var()
{
    string s = "%t" + to_string(this->tmp_var_label++);
    var_declare_stream << "\tdeclare i32 " << s << endl;
    return s;
}