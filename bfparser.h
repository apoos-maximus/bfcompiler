#pragma once
#include <vector>
#include "token_class.h"
#include "bfspec.h"

class bfparser
{
private:
    vector<token_class> token_stream;
    token_class curtok;
    int parse_head;

public:
    bfparser(vector<token_class>);
    bfparser();
    ~bfparser();
    bool parse();
    void prog();
    void loop();
    vector<token_class> get_token_stream();
    token_class next_token();
};
