#include "bfnode.h"

//progAST
progAST ::~progAST()
{
}

void progAST ::print()
{
    cout << "lol";
}

void progAST ::set_next(progAST *nn)
{
    this->next = nn;
}

progAST *progAST ::get_next()
{
    return this->next;
}

//operatorAST
operatorAST ::operatorAST(char value) : value(value)
{
    this->next = NULL;
}

char operatorAST ::get_value()
{
    return this->value;
}

void operatorAST ::print()
{
    cout << this->value;
    if (this->next != NULL)
        this->next->print();
    else
        return;
}

//loopAST
loopAST ::loopAST(progAST *internal) : loop_content(internal)
{
    this->next = NULL;
}
progAST *loopAST ::get_loop_content()
{
    return this->loop_content;
}

void loopAST::print()
{
    cout << '[';
    if (loop_content != NULL)
        loop_content->print();
    cout << ']';
    if (this->next != NULL)
        this->next->print();
    else
        return;
}