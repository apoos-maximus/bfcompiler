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
    this->bfast = prog();
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

progAST *bfparser::prog()
{
    progAST *retval = NULL;
    if ((curtok.get_type() == INCR) || (curtok.get_type() == DECR) || (curtok.get_type() == FWD) || (curtok.get_type() == BKWD) || (curtok.get_type() == INPUT) || (curtok.get_type() == OUTPUT))
    {
        retval = new operatorAST(curtok.get_value());
        curtok = next_token();
        retval->set_next(prog());
        return retval;
    }
    if (curtok.get_type() == LOP)
    {
        return loop();
    }
    if (curtok.get_type() == END_SENTINAL)
    {
        return NULL;
    }
    return NULL;
}

progAST *bfparser::loop()
{
    progAST *retval = NULL;
    if (curtok.get_type() == LOP)
    {
        curtok = next_token();
        retval = new loopAST(prog());

        if (curtok.get_type() == LOPIF)
        {
            curtok = next_token();
            retval->set_next(prog());
            return retval;
        }
        else
        {
            cout << "missing ]" << endl;
            exit(0);
        }
        return NULL;
    }
    return NULL;
}

vector<token_class> bfparser::get_token_stream()
{
    return this->token_stream;
}

progAST *bfparser ::getAST()
{
    return this->bfast;
}

token_class bfparser::next_token()
{
    parse_head++;
    return token_stream[parse_head];
}