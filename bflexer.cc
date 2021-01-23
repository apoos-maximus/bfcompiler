#include "bflexer.h"

bflexer ::bflexer(string raw_stream)
{
    this->raw_stream = raw_stream;
}

bflexer ::~bflexer()
{
}

string bflexer ::get_raw_stream()
{
    return this->raw_stream;
}

vector<token_class> bflexer ::get_tokenized_stream()
{
    return this->tokenized_stream;
}
void bflexer ::lexit()
{
    int i = 0;
    char ch = '~';
    while (i < raw_stream.size())
    {
        ch = this->raw_stream[i];
        switch (ch)
        {
        case '+':
            this->tokenized_stream.push_back(token_class('+', INCR));
            i++;
            break;
        case '-':
            this->tokenized_stream.push_back(token_class('-', DECR));
            i++;
            break;
        case '>':
            this->tokenized_stream.push_back(token_class('>', FWD));
            i++;
            break;
        case '<':
            this->tokenized_stream.push_back(token_class('<', BKWD));
            i++;
            break;
        case '[':
            this->tokenized_stream.push_back(token_class('[', LOP));
            i++;
            break;
        case ']':
            this->tokenized_stream.push_back(token_class(']', LOPIF));
            i++;
            break;
        case ',':
            this->tokenized_stream.push_back(token_class(',', INPUT));
            i++;
            break;
        case '.':
            this->tokenized_stream.push_back(token_class('.', OUTPUT));
            i++;
            break;
        default:
            i++;
            break;
        }
    }
}