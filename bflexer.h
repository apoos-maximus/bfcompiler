#pragma once
#include "bfspec.h"
#include "token_class.h"
#include <string>
#include <vector>

class bflexer
{
private:
    string raw_stream;
    vector<token_class> tokenized_stream;

public:
    bflexer(string raw_stream);
    ~bflexer();
    string get_raw_stream();
    vector<token_class> get_tokenized_stream();
    void lexit();
};