#include "token_class.h"
#include "bfspec.h"

token_class ::token_class(char value, token_type type)
{
    this->value = value;
    this->type = type;
}

char token_class ::get_value()
{
    return this->value;
}
token_type token_class ::get_type()
{
    return this->type;
}
void token_class ::set_value(char value)
{
    this->value = value;
}

void token_class ::set_type(token_type type)
{
    this->type = type;
}
