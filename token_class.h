#pragma once
#include "bfspec.h"

class token_class
{
private:
    char value;
    token_type type;

public:
    token_class(char value, token_type type);
    token_class();

    char get_value();
    token_type get_type();
    void set_value(char value);
    void set_type(token_type type);
};