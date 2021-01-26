#pragma once
#include <vector>
#include "token_class.h"
#include "bfspec.h"
#include "bfnode.h"

class bfparser
{
private:
    vector<token_class> token_stream;
    token_class curtok;
    int parse_head;
    progAST *bfast;

public:
    bfparser(vector<token_class>);
    bfparser();
    ~bfparser();
    bool parse();
    progAST *prog();
    progAST *loop();
    vector<token_class> get_token_stream();
    token_class next_token();
    progAST *getAST();
};
