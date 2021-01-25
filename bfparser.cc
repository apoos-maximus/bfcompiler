#include "bfparser.h"
#include <iostream>
bfparser::bfparser()
{
}
bfparser::bfparser(vector<token_class> token_stream)
{
    this->token_stream = token_stream;
    this->parse_head = -1;
    this->curtok = token_class(characterset[END_SENTINAL], END_SENTINAL); //insignificant dumb initialization
}

bfparser::~bfparser()
{
}

bool bfparser::parse()
{
    curtok = next_token();
    prog();
    if (curtok.get_type() == END_SENTINAL)
    {
        return true;
    }
    else
    {
        if (curtok.get_type() == LOPIF)
        {
            cout << "missing [" << endl;
        }
        return false;
    }
}

void bfparser::prog()
{
    if ((curtok.get_type() == INCR) || (curtok.get_type() == DECR) || (curtok.get_type() == FWD) || (curtok.get_type() == BKWD) || (curtok.get_type() == INPUT) || (curtok.get_type() == OUTPUT))
    {
        curtok = next_token();
        prog();
        return;
    }
    if (curtok.get_type() == LOP)
    {
        loop();
        return;
    }
    if (curtok.get_type() == END_SENTINAL)
    {
        return;
    }
    return;
}

void bfparser::loop()
{
    if (curtok.get_type() == LOP)
    {
        curtok = next_token();
        prog();
        if (curtok.get_type() == LOPIF)
        {
            curtok = next_token();
            prog();
            return;
        }
        else
        {
            cout << "missing ]" << endl;
            exit(0);
        }
        return;
    }
}

vector<token_class> bfparser::get_token_stream()
{
    return this->token_stream;
}

token_class bfparser::next_token()
{
    parse_head++;
    return token_stream[parse_head];
}
