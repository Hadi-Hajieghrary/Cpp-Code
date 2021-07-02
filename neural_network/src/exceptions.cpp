#include "exceptions.hpp"


Dimension_MisMatch::Dimension_MisMatch(int arg1, int arg2)
    :arg1_{arg1}, arg2_{arg2}
    {

    }

char* Dimension_MisMatch::what(){
    char* msg = new char[100];
    int res = sprintf(msg, "Dimension mismatch: %d v.s. %d", arg1_, arg2_);
    return msg;
}