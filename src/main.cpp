#include "common.h"
#include <fstream>

extern TreeNode *root;
extern FILE *yyin;
extern int yyparse();
extern stringstream finalstream;
extern multimap<string, string> idNameList;
bool parserError = false;
bool typeError = false;

using namespace std;
int main(int argc, char *argv[])
{
    fstream f;
    if (argc >= 2)
    {
        string s = string(argv[1]);
        if (s == "-i")
        {
            FILE *fin = fopen(argv[4], "r");
            if (fin != nullptr)
            {
                yyin = fin;
            }
            else
            {
                cerr << "failed to open file: " << argv[1] << endl;
            }
            f.open(argv[3], ios::in | ios::out | ios::trunc);
        }
        else
        {
            cerr << "arg error" << endl;
        }
    }

    yyparse();

    if (parserError)
        return 0;

    root->genNodeId();

    if (typeError)
        return 0;

    root->genCode();
    string s;
    while (getline(finalstream, s))
    {
        f << s << endl;
    }
    f.close();
    root->printAST();
    cout << "变量表" << endl;
    auto var_it = idNameList.begin();
    while (var_it != idNameList.end())
    {
        cout << "id: " << var_it->first << "\tscope:" << var_it->second << endl;
        var_it++;
    }
    return 0;
}
