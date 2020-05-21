#ifndef trog_sample_class_d_hpp
#define trog_sample_class_d_hpp
#include <string>
#include <ostream>
#include "configure_trog.hpp"
/** this file is playing the role of a header only library wanting to use Trog*/
class DClass
{
public:
    std::string m_something;
    DClass(std::string something): m_something(something)
    {
        TROG_TRACE_CTOR();
    }

    std::string d_func(int i)
    {
        TROG_DEBUG("The argument: ", i);
        TROG_TRACE3("TRACE3 this will not display", i);
        TROG_TRACE4("TRACE4 this will display - reverse of AClass", i);

        std::ostringstream os;

        os << __PRETTY_FUNCTION__ << " something: " << m_something << " argument: " << i;

        std::string s = os.str();
        TROG_DEBUG("The return value : ", s);
        return s;
    }

    ~DClass()
    {
        TROG_TRACE_CTOR();
    }
};
#endif