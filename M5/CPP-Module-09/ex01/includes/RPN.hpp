#ifndef RPN_HPP
#define RPN_HPP

#include "Colors.hpp"
#include <iostream>
#include <stack>

class RPN
{
    private:
        std::stack<int> stk;
    public:
        RPN();
        ~RPN();
        void evaluate(const std::string& expression);
};

#endif
