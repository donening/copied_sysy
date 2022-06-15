#include "tree.h"

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
#ifdef childNumdebug
        cout << "# ChildNum = " << N << endl;
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
        cout << "call " << it->second->getTypeInfo() << " @" << it->first;
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
            func_new->return_var = type->type == VALUE_INT ? "%t" + this->child->sibling->var_name + "_return" : "";
            func_stack.push(func_new);
            // 生成返回标签
            this->get_label();
            // 生成局部变量定义表
            this->gen_var_decl();
            // 确定返回值类型
            string print_return_type = type->type == VALUE_INT ? "i32 @" : "void @";
            // 生成函数声明头
            cout << "declare " << print_return_type << this->child->sibling->var_name;
            // 生成参数列表
            cout << "(";
            for (auto iter_def_var = LocalVarList.begin(); iter_def_var->second->nodeType == NODE_PARAM; iter_def_var++)
            {
                if (iter_def_var != LocalVarList.begin())
                    cout << ", ";
                string labels = iter_def_var->first;
                cout << iter_def_var->second->type->getTypeInfo() << " %t"
                     << "tmp" << labels;
            }
            cout << "){" << endl;
            // 生成参数declare语句
            for (auto iter_def_var = LocalVarList.begin(); iter_def_var != LocalVarList.end(); iter_def_var++)
            {
                cout << "\tdeclare " << iter_def_var->second->type->getTypeInfo();
                cout << " %l" << iter_def_var->first;
                if (iter_def_var->second->type->dim != 0)
                {
                    for (unsigned int i = 0; i < iter_def_var->second->type->dim; i++)
                        cout << "[" << iter_def_var->second->type->dimSize[i] << "]";
                }
            }
            // 生成函数参数初始化语句
            for (auto iter_def_var = LocalVarList.begin(); iter_def_var->second->nodeType == NODE_PARAM; iter_def_var++)
            {
                cout << "\t%l" << iter_def_var->first << " = "
                     << "%ttmp" << iter_def_var->first;
            }
            // 内部代码递归生成
            p->sibling->sibling->sibling->genCode();
            // 产生返回标签代码
            cout << this->label.next_label << ":" << endl;
            cout << " exit " << func_stack.top()->return_var << endl;
            cout << "}" << endl;
            func_stack.pop();
            pFunction = nullptr;
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
            //         cout << "\tmovl\t%eax, " << LocalVarList[p->child->var_scope + p->child->var_name] << "(%ebp)" << endl;
            //     }
            //     p = p->sibling;
            // }
            break;
        case STMT_IF:
            get_label();
            cout << label.begin_label << ":" << endl;
            this->child->genCode();
            cout << label.true_label << ":" << endl;
            this->child->sibling->genCode();
            cout << label.false_label << ":" << endl;
            break;
        case STMT_IFELSE:
            get_label();
            cout << label.begin_label << ":" << endl;
            this->child->genCode();
            cout << label.true_label << ":" << endl;
            this->child->sibling->genCode();
            cout << "\tbr " << label.next_label << endl;
            cout << label.false_label << ":" << endl;
            this->child->sibling->sibling->genCode();
            cout << label.next_label << ":" << endl;
            break;
        case STMT_WHILE:
            get_label();
            cycleStack[++cycleStackTop] = this;
            cout << label.next_label << ":" << endl;
            this->child->genCode();
            cout << label.true_label << ":" << endl;
            this->child->sibling->genCode();
            cout << "\tbr " << label.next_label << endl;
            cout << label.false_label << ":" << endl;
            cycleStackTop--;
            break;
        case STMT_BREAK:
            cout << "\tbr " << cycleStack[cycleStackTop]->label.false_label << endl;
            break;
        case STMT_CONTINUE:
            cout << "\tbr " << cycleStack[cycleStackTop]->label.next_label << endl;
            break;
        case STMT_RETURN:
            if (p)
            {
                p->genCode();
            }
            cout << "\tbr " << pFunction->label.next_label << endl;
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
                cout << "%g" << varCode;
            else if (child->pointLevel < 0)
            { // &前缀的变量
                cout << "\tleal\t" << varCode << ", %eax" << endl;
            }
            else
            {
                cout << "\tmovl\t" << varCode << ", %eax" << endl;
                for (int i = 0; i < child->pointLevel; i++)
                {
                    cout << "\tmovl\t(%eax), %eax" << endl;
                }
            }
        }
        else if (child->nodeType == NODE_OP && child->optype == OP_INDEX)
        {
            // 数组
            child->genCode();
        }
        else
        {
            cout << "\tmovl\t$" << child->getVal() << ", %eax" << endl;
        }
        break;
    case NODE_OP:
        switch (optype)
        {
        case OP_EQ:
            p->genCode();
            cout << "\tpushl\t%eax" << endl;
            p->sibling->genCode();
            cout << "\tpopl\t%ebx" << endl
                 << "\tcmpl\t%eax, %ebx" << endl
                 << "\tsete\t%al" << endl;
            if (label.true_label != "")
            {
                cout << "\tje\t\t" << label.true_label << endl
                     << "\tbr\t\t" << label.false_label << endl;
            }
            break;
        case OP_NEQ:
            p->genCode();
            cout << "\tpushl\t%eax" << endl;
            p->sibling->genCode();
            cout << "\tpopl\t%ebx" << endl
                 << "\tcmpl\t%eax, %ebx" << endl
                 << "\tsetne\t%al" << endl;
            if (label.true_label != "")
            {
                cout << "\tjne\t\t" << label.true_label << endl
                     << "\tbr\t\t" << label.false_label << endl;
            }
            break;
        case OP_GRA:
            p->genCode();
            cout << "\tpushl\t%eax" << endl;
            p->sibling->genCode();
            cout << "\tpopl\t%ebx" << endl
                 << "\tcmpl\t%eax, %ebx" << endl
                 << "\tsetg\t%al" << endl;
            if (label.true_label != "")
            {
                cout << "\tjg\t\t" << label.true_label << endl
                     << "\tbr\t\t" << label.false_label << endl;
            }
            break;
        case OP_LES:
            p->genCode();
            cout << "\tpushl\t%eax" << endl;
            p->sibling->genCode();
            cout << "\tpopl\t%ebx" << endl
                 << "\tcmpl\t%eax, %ebx" << endl
                 << "\tsetl\t%al" << endl;
            if (label.true_label != "")
            {
                cout << "\tjl\t\t" << label.true_label << endl
                     << "\tbr\t\t" << label.false_label << endl;
            }
            break;
        case OP_GRAEQ:
            p->genCode();
            cout << "\tpushl\t%eax" << endl;
            p->sibling->genCode();
            cout << "\tpopl\t%ebx" << endl
                 << "\tcmpl\t%eax, %ebx" << endl
                 << "\tsetge\t%al" << endl;
            if (label.true_label != "")
            {
                cout << "\tjge\t\t" << label.true_label << endl
                     << "\tbr\t\t" << label.false_label << endl;
            }
            break;
        case OP_LESEQ:
            p->genCode();
            cout << "\tpushl\t%eax" << endl;
            p->sibling->genCode();
            cout << "\tpopl\t%ebx" << endl
                 << "\tcmpl\t%eax, %ebx" << endl
                 << "\tsetle\t%al" << endl;
            if (label.true_label != "")
            {
                cout << "\tjle\t\t" << label.true_label << endl
                     << "\tbr\t\t" << label.false_label << endl;
            }
            break;
        case OP_NOT:
            get_label();
            p->genCode();
            // cout << "\tandl\t%eax, $eax" << endl
            //      << "\tsete\t%al" << endl;
            break;
        case OP_AND:
            get_label();
            p->genCode();
            cout << "\tpushl\t%eax" << endl;
            cout << child->label.true_label << ":" << endl;
            p->sibling->genCode();
            cout << "\tpopl\t%ebx" << endl
                 << "\tandl\t%eax, %ebx" << endl
                 << "\tsetne\t%al" << endl;
            break;
        case OP_OR:
            get_label();
            p->genCode();
            cout << "\tpushl\t%eax" << endl;
            cout << child->label.false_label << ":" << endl;
            p->sibling->genCode();
            cout << "\tpopl\t%ebx" << endl
                 << "\torb\t%al, %bl" << endl
                 << "\tsetne\t%al" << endl;
            break;
        case OP_ADDASSIGN:
            varCode = getVarNameCode(p);
            p->sibling->genCode();

            cout << "\tmovl\t" << varCode << ", %ebx" << endl
                 << "\taddl\t%ebx, %eax" << endl
                 << "\tmovl\t%eax, " << varCode << endl;
            break;
        case OP_SUBASSIGN:
            varCode = getVarNameCode(p);
            p->sibling->genCode();
            cout << "\tmovl\t" << varCode << ", %ebx" << endl
                 << "\tsubl\t%eax, %ebx" << endl
                 << "\tmovl\t%ebx, %eax" << endl
                 << "\tmovl\t%eax, " << varCode << endl;
            break;
        case OP_MULASSIGN:
            varCode = getVarNameCode(p);
            p->sibling->genCode();
            cout << "\tmovl\t" << varCode << ", %ebx" << endl
                 << "\timull\t%ebx, %eax" << endl
                 << "\tmovl\t%eax, " << varCode << endl;
            break;
        case OP_DIVASSIGN:
        {
            string var_name = getVarNameCode(p);
            p->sibling->genCode();
            string target = gen_var_label_stack.top();
            gen_var_label_stack.pop();
            cout << "\t%l" << var_name << " = " << target << endl;
        }
        break;
        case OP_ASSIGN:
            p->sibling->genCode();
            if (p->nodeType == NODE_VAR)
                cout << "\tmovl\t%eax, " << getVarNameCode(p) << endl;
            else
            { // 左值是数组
                cout << "\tpushl\t%eax" << endl;
                // 计算偏移量到%eax
                p->child->sibling->genCode();
                cout << "\tpopl\t%ebx" << endl
                     << "\tmovl\t%ebx, " << getVarNameCode(p) << endl;
            }
            break;
        case OP_INC:
            varCode = getVarNameCode(p);
            cout << "\t%l" << varCode << " = %l" << varCode << " + 1" << endl;
            break;
        case OP_DEC:
            varCode = getVarNameCode(p);
            cout << "\t%l" << varCode << " = %l" << varCode << " - 1" << endl;
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
            cout << "\t" << tmp_var_label << " = add " << a << " " << b << endl;
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
            cout << "\t" << tmp_var_label << " = sub " << a << " " << b << endl;
            gen_var_label_stack.push(tmp_var_label);
        }
        break;
        case OP_POS:
            p->genCode();
            break;
        case OP_NAG:
        {
            p->genCode();
            string neg = gen_var_label_stack.top();
            cout << "\tnegl\t%eax" << endl;
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
            cout << "\t" << tmp_var_label << " = mul " << a << " " << b << endl;
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
            cout << "\t" << tmp_var_label << " = div " << a << " " << b << endl;
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
            cout << "\t" << tmp_var_label << " = mod " << a << " " << b << endl;
            gen_var_label_stack.push(tmp_var_label);
        }
        break;
        case OP_INDEX:
            // 这里只生成下标运算在右值时的代码（即按下标取数值）
            p->sibling->genCode();
            cout << "\tmovl\t" << getVarNameCode(this) << ", %eax" << endl;
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
                    cout << "declare i32 @" << t->var_name;
                    if (t->type->dim != 0)
                    {
                        for (unsigned int i = 0; i < t->type->dim; i++)
                            cout << "[" << t->type->dimSize[i] << "]";
                    }
                    cout << endl;
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
        cout << "# gen_var_decl in funcDecl init" << endl;
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
        cout << "# gen_var_decl in funcDecl param fin" << endl;
#endif
        // 遍历代码段，查找函数内声明的局部变量
        p = child->sibling->sibling->sibling->child;
        while (p)
        {
            p->gen_var_decl();
            p = p->sibling;
        }
#ifdef varDeclDebug
        cout << "# gen_var_decl in funcDecl fin" << endl;
#endif
    }
    else if (nodeType == NODE_STMT && (stype == STMT_DECL || stype == STMT_CONSTDECL))
    {
#ifdef varDeclDebug
        cout << "# gen_var_decl found varDecl stmt at node " << nodeID << endl;
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

string TreeNode::getVarNameCode(TreeNode *p)
{
    string varCode = "";
    if (p->nodeType == NODE_VAR)
    {
        // 标识符
        if (p->var_scope == "1")
        {
            // 全局变量
            varCode = p->var_name;
        }
        else
        {
            // 局部变量（不要跨定义域访问）
            varCode = p->var_scope + p->var_name;
        }
    }
    else
    {
        // 数组
        if (p->child->var_scope == "1")
        {
            varCode = p->child->var_name + "(,%eax,4)";
        }
        else
        {
            varCode += LocalVarList[p->child->var_scope + p->child->var_name]->var_name;
            varCode += "(%ebp,%eax,4)";
        }
    }
    return varCode;
}

void TreeNode::printAST()
{
    printNodeInfo();
    printChildrenId();
    cout << endl;
    TreeNode *p = this->child;
    while (p != nullptr)
    {
        p->printAST();
        p = p->sibling;
    }
}

void TreeNode::printNodeInfo()
{
    cout << "# @" << this->nodeID << "\t";
    cout << "line " << lineno << "\t";
    cout << nodeType2String(this->nodeType);
    this->printSpecialInfo();
}

void TreeNode::printChildrenId()
{
    cout << ",\tchild:";
    TreeNode *p = this->child;
    if (p == nullptr)
        cout << "-";
    while (p)
    {
        cout << "\t@" << p->nodeID;
        p = p->sibling;
    }
    cout << "\t";
}

void TreeNode::printSpecialInfo()
{
    switch (this->nodeType)
    {
    case NODE_CONST:
        cout << ", ";
        this->printConstVal();
        break;
    case NODE_VAR:
        cout << ",\tname: ";
        if (this->type->pointLevel != 0)
        {
            // 为指针类型添加前缀(*和&)
            string prefix = this->type->pointLevel > 0 ? "*" : "&";
            for (int i = 0; i < abs(this->type->pointLevel); i++)
                cout << prefix;
        }
        cout << var_name << ",\tscope: ";
        for (unsigned int i = 0; i < var_scope.length(); i++)
            cout << var_scope[i] << " ";
        break;
    case NODE_EXPR:
        break;
    case NODE_STMT:
        cout << ", " << sType2String(this->stype) << "\t";
        if (this->stype == STMT_DECL || this->stype == STMT_CONSTDECL)
        {
            if (this->child && this->child->sibling && this->child->sibling->type)
                cout << this->child->sibling->type->getTypeInfo() << "\t";
        }
        break;
    case NODE_TYPE:
        cout << ", " << this->type->getTypeInfo();
        break;
    case NODE_OP:
        cout << ", " << opType2String(this->optype) << "\t";
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
        cout << this->type->getTypeInfo() << ":";
        switch (this->type->type)
        {
        case VALUE_INT:
            cout << int_val;
            break;
        default:
            cout << "-";
            break;
        }
    }
}
// 直接生成%t[name]格式的临时变量字符串
string Function::Ask_tmp_var()
{
    return "%t" + to_string(this->tmp_var_label++);
}