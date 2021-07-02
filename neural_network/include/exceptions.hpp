#ifndef __EXCEPTOINS_
#define __EXCEPTOINS_

#include <stdexcept>
#include <cstdio>


class Dimension_MisMatch: public std::exception{
    private:
        int arg1_{0}, arg2_{0};
    protected:

    public:

        Dimension_MisMatch(int arg1, int arg2);

        char* what();
};

#endif