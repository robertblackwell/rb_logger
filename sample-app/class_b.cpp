#include <trog/loglevel.hpp>
#define TROG_FILE_LEVEL (TROG_LEVEL_CTOR | TROG_LEVEL_VERBOSE)
#include "configure_trog.hpp"

#include "class_b.hpp"

#include <string>
#include <sstream>

BClass::BClass()
{
    TROG_TRACE_CTOR();
}
std::string BClass::b_func(int i)
{
    TROG_DEBUG("The argument: ", i);
    TROG_TRACE3("TRACE3 this will not display", i);
    TROG_TRACE4("TRACE4 this will display - reverse of AClass", i);

    std::ostringstream os;

    os << __PRETTY_FUNCTION__ << " argument: " << i;
    
    std::string s = os.str();
    TROG_DEBUG("The return value : ", s);
    return s;
}
 BClass::~BClass()
{
    TROG_TRACE_CTOR();
}
