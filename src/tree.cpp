#include "tree.h"

// -------------------------------------------------------------
// ------------------------- 常量字符串 -------------------------
// -------------------------------------------------------------

string _ident = "\"LuczyCompiler: (Ubuntu 1.0.0ubuntu1~18.04) 1.0.0\"";
string _section = ".note.GNU-stack,\"\",@progbits";

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

// map <字符串， 标签序列号> 字符串表
map<string, int> strList;

// map <作用域+变量名， 变量相对于ebp偏移量> 局部变量表，在每次函数定义前清空
// <"11a", "-12"> 表示第一个函数的栈上第一个分配的局部变量（前3个4字节为bx,cx,dx备份用，始终保留）
map<string, int> LocalVarList;
// 栈上为局部变量分配的空间总大小，在return时进行清理
int stackSize;

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
    this->str_val = node->str_val;
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
    TreeNode *p = child;
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
        auto tmp_var_lists = new vector<string>(N);
#ifdef childNumdebug
        cout << "# ChildNum = " << N << endl;
#endif
        q = new TreeNode *[N];
        p = p->sibling->child;
        while (p)
        {
            q[N - n++] = p;
            p = p->sibling;
        }
        // 从右向左压栈
        for (int i = 0; i < N; i++)
        {
            q[i]->genCode();
            cout << "\tpushl\t%eax" << endl;
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
            get_label();
            cout << "\t.globl\t" << p->sibling->var_name << endl
                 << "\t.type\t" << p->sibling->var_name << ", @function" << endl
                 << p->sibling->var_name << ":" << endl;
            gen_var_decl();
            cout << "\tpushl\t%ebp" << endl
                 << "\tmovl\t%esp, %ebp" << endl;
            // 在栈上分配局部变量
            cout << "\tsubl\t$" << -stackSize << ", %esp" << endl;
            // 内部代码递归生成
            p->sibling->sibling->sibling->genCode();
            // 产生返回标签代码
            cout << this->label.next_label << ":" << endl;
            // 清理局部变量栈空间
            cout << "\taddl\t$" << -stackSize << ", %esp" << endl;
            cout << "\tpopl\t%ebp" << endl
                 << "\tret" << endl;
            pFunction = nullptr;
        }
        break;
        case STMT_DECL:
        case STMT_CONSTDECL:
            p = p->sibling->child;
            while (p)
            {
                if (p->nodeType == NODE_OP)
                {
                    p->child->sibling->genCode();
                    // 这里也很蠢，可以通过三地址码优化一下
                    cout << "\tmovl\t%eax, " << LocalVarList[p->child->var_scope + p->child->var_name] << "(%ebp)" << endl;
                }
                p = p->sibling;
            }
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
            cout << "\tbr\t\t" << label.next_label << endl;
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
            cout << "\tbr\t\t" << label.next_label << endl;
            cout << label.false_label << ":" << endl;
            cycleStackTop--;
            break;
        case STMT_FOR:
            get_label();
            cycleStack[++cycleStackTop] = this;
            this->child->genCode();
            cout << label.begin_label << ":" << endl;
            this->child->sibling->genCode();
            cout << label.true_label << ":" << endl;
            this->child->sibling->sibling->sibling->genCode();
            cout << label.next_label << ":" << endl;
            this->child->sibling->sibling->genCode();
            cout << "\tbr\t\t" << label.begin_label << endl;
            cout << label.false_label << ":" << endl;
            cycleStackTop--;
            break;
        case STMT_BREAK:
            cout << "\tbr\t\t" << cycleStack[cycleStackTop]->label.false_label << endl;
            break;
        case STMT_CONTINUE:
            cout << "\tbr\t\t" << cycleStack[cycleStackTop]->label.next_label << endl;
            break;
        case STMT_RETURN:
            if (p)
            {
                p->genCode();
            }
            cout << "\tbr\t\t" << pFunction->label.next_label << endl;
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
                cout << "\tmovl\t" << varCode << ", %eax" << endl;
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
            varCode = getVarNameCode(p);
            p->sibling->genCode();
            cout << "\tmovl\t%eax, %ebx" << varCode << endl
                 << "\tmovl\t" << varCode << ", %eax" << endl
                 << "\tcltd" << endl
                 << "\tidivl\t%ebx" << endl
                 << "\tmovl\t%eax, " << varCode << endl;
            break;
        case OP_DECLASSIGN:
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
            cout << "\tmovl\t" << varCode << ", %eax" << endl
                 << "\tincl\t" << varCode << endl;
            break;
        case OP_DEC:
            varCode = getVarNameCode(p);
            cout << "\tmovl\t" << varCode << ", %eax" << endl
                 << "\tdecl\t" << varCode << endl;
            break;
        case OP_ADD:
            p->genCode();
            cout << "\tpushl\t%eax" << endl;
            p->sibling->genCode();
            cout << "\tpopl\t%ebx" << endl;
            cout << "\taddl\t%ebx, %eax" << endl;
            break;
        case OP_SUB:
            p->genCode();
            cout << "\tpushl\t%eax" << endl;
            p->sibling->genCode();
            cout << "\tmovl\t%eax, %ebx" << endl
                 << "\tpopl\t%eax" << endl
                 << "\tsubl\t%ebx, %eax" << endl;
            break;
        case OP_POS:
            p->genCode();
            break;
        case OP_NAG:
            p->genCode();
            cout << "\tnegl\t%eax" << endl;
            break;
        case OP_MUL:
            p->genCode();
            cout << "\tpushl\t%eax" << endl;
            p->sibling->genCode();
            cout << "\tpopl\t%ebx" << endl;
            cout << "\timull\t%ebx, %eax" << endl;
            break;
        case OP_DIV:
            p->genCode();
            cout << "\tpushl\t%eax" << endl;
            p->sibling->genCode();
            cout << "\tmovl\t%eax, %ebx" << endl
                 << "\tpopl\t%eax" << endl
                 << "\tcltd" << endl
                 << "\tidivl\t%ebx" << endl;
            break;
        case OP_MOD:
            p->genCode();
            cout << "\tpushl\t%eax" << endl;
            p->sibling->genCode();
            cout << "\tmovl\t%eax, %ebx" << endl
                 << "\tpopl\t%eax" << endl
                 << "\tcltd" << endl
                 << "\tidivl\t%ebx" << endl
                 << "\tmovl\t%edx, %eax" << endl;
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
                    if (q->nodeType == NODE_OP && q->optype == OP_DECLASSIGN)
                    {
                        t = q->child;
                    }
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
        stackSize = -12;
        int paramSize = 8;
#ifdef varDeclDebug
        cout << "# gen_var_decl in funcDecl init" << endl;
#endif
        // 遍历参数定义列表
        TreeNode *p = child->sibling->sibling->child;
        while (p)
        {
            // 只能是基本数据类型，简便起见一律分配4字节
            LocalVarList[p->child->sibling->var_scope + p->child->sibling->var_name] = paramSize;
            paramSize += 4;
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
            if (q->nodeType == NODE_OP && q->optype == OP_DECLASSIGN)
                t = q->child;
            int varsize = ((t->type->pointLevel == 0) ? t->type->getSize() : 4);
            if (t->type->dim > 0)
            {
                t->type->type = VALUE_ARRAY;
                varsize = t->type->getSize();
            }
            LocalVarList[t->var_scope + t->var_name] = stackSize;
            stackSize -= varsize;
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
        case STMT_FOR:
            this->label.begin_label = new_label();
            this->label.true_label = new_label();
            this->label.false_label = new_label();
            this->label.next_label = new_label();
            this->child->sibling->label.true_label = this->label.true_label;
            this->child->sibling->label.false_label = this->label.false_label;
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
            varCode += LocalVarList[p->var_scope + p->var_name];
            varCode += "(%ebp)";
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
            varCode += LocalVarList[p->child->var_scope + p->child->var_name];
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
    case OP_DECLASSIGN:
        return "assign(decl)";
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
