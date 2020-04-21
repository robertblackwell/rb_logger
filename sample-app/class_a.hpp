#ifndef trog_sample_class_a_hpp
#define trog_sample_class_a_hpp
#include <string>
#include <ostream>

///
/// If you are OK with the default configuration of Trog
/// #include <trog/trog.hpp>
/// On the other hand if you want to customize Trog's behavious
/// make an include file called configure_trog.hpp
/// and include it instead of trog.hpp
///
#ifdef OVERRIDE_TROG
#include "configure_trog.hpp"
#else
#include <trog/trog.hpp>
#endif

class AClass
{
    public:
    AClass();
    std::string a_func(int i);
    ~AClass();
};

#endif