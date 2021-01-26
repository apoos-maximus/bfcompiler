#pragma once
#include <iostream>
#include "bfspec.h"

// progAST class
class progAST
{
public:
    progAST *next;
    void set_next(progAST *nn);
    progAST *get_next();
    virtual ~progAST();
    virtual void print();
};

// operatorAST
class operatorAST : public progAST
{
    char value;

public:
    operatorAST(char value);
    char get_value();
    void print();
};

// loopAST
class loopAST : public progAST
{
    progAST *loop_content;

public:
    loopAST(progAST *internal);
    progAST *get_loop_content();
    void print();
};