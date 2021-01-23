#include <iostream>
#include <fstream>
#include "token_class.h"
#include "bflexer.h"

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
    for (vector<token_class>::iterator iter = sanitised.begin(); iter != sanitised.end(); iter++)
    {
        cout << characterset[(*iter).get_type()];
    }
    cout << endl;
    return 0;
}