#include<iostream>
#include "token_class.h"

int main () {
    token_class *tk = new token_class('[',INCR);
    cout << tk->get_value()<<endl;
    return 0;
}