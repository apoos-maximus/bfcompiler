#pragma once
#include <set>

using namespace std;

const char characterset[] = {
    '+',
    '-',
    '>',
    '<',
    '[',
    ']',
    ',',
    '.'};

enum token_type
{
    INCR,  // +
    DECR,  // -
    FWD,   // >
    BKWD,  // <
    LOP,   // [
    LOPIF, // ]
    INPUT, // ,
    OUTPUT // .
};
