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
    '.',
    '$'};

enum token_type
{
    INCR,        // +
    DECR,        // -
    FWD,         // >
    BKWD,        // <
    LOP,         // [
    LOPIF,       // ]
    INPUT,       // ,
    OUTPUT,      // .
    END_SENTINAL // $                                                                      //character denotes end of parsing
};

/*
 * Context Free Grammer for Brainfuck :
 * 
 * TOP      ->      PROG
 * PROG     ->      (LOOP | OPS) PROG || epsilon                                           // epsilon = empty production
 * OPS      ->      (INCR | DECR | FWD | BKWD | INPUT | OUTPUT)
 * LOOP     ->      LOP PROG LOPIF
 * INCR     ->      '+'
 * DECR     ->      '-'
 * FWD      ->      '>'
 * BKWD     ->      '<'
 * INPUT    ->      ','
 * OUTPUT   ->      '.'
 * LOP      ->      '['
 * LOPIF    ->      ']'
 */

/*
 * Reduced Brainfuck Grammer :
 * 
 * TOP      ->      PROG
 * PROG     ->      [ PROG ] PROG |
 *          ->      + PROG
 *          ->      - PROG
 *          ->      > PROG
 *          ->      < PROG
 *          ->      , PROG
 *          ->      . PROG
 *          ->      epsilon                                                                // epsilon = empty production                         
 */