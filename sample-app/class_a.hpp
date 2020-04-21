#ifndef trog_sample_class_a_hpp
#define trog_sample_class_a_hpp
#include <string>
#include <ostream>

#include "configure_trog.hpp"

class AClass
{
    public:
    AClass();
    std::string a_func(int i);
    ~AClass();
};

#endif