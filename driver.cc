#include <iostream>
#include <fstream>
#include "token_class.h"
#include "bflexer.h"
#include "bfparser.h"
#include "bfnode.h"
#include "IRgenerator.h"

int main(int argc, char *argv[])
{
    string cnt = "";
    string line;
    ifstream MyReadFile(argv[1]);
    while (getline(MyReadFile, line))
    {
        cnt += line;
    }
    MyReadFile.close();

    bflexer lexer = bflexer(cnt);
    lexer.lexit();
    vector<token_class> sanitised = lexer.get_tokenized_stream();
    sanitised.push_back(token_class(characterset[END_SENTINAL], END_SENTINAL));
    bfparser parser = bfparser(sanitised);

    vector<token_class> a = parser.get_token_stream();
    for (vector<token_class>::iterator iter = a.begin(); iter != a.end(); iter++)
    {
        cout << characterset[(*iter).get_type()];
    }
    cout << endl;
    if (parser.parse())
    {
        cout << "parse success !" << endl;
    }
    else
    {
        cout << "parse error !" << endl;
        exit(-1);
    }

    progAST *tr = parser.getAST();
    tr->print();
    cout << endl;

    IRgenerator irg = IRgenerator(tr);
    irg.codegen();

    return 0;
}