#ifndef trog_sample_class_b_hpp
#define trog_sample_class_b_hpp
#include <string>
#include <ostream>
#include <trog/trog.hpp>
#include "configure_trog.hpp"

class BClass
{
    public:
    BClass();
    std::string b_func(int i);
    ~BClass();
};

#endif