#include "class_b.hpp"

#include <string>
#include <sstream>
#include <trog/trog.hpp>

// if this line is not present this file will not compile as variable TrogFileLevel
// will not be defined
TROG_SET_FILE_LEVEL(Trog::LogLevelCTorTrace|Trog::LogLevelTrace4|Trog::LogLevelVerbose);

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
