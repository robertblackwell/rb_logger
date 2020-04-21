#include "class_a.hpp"

#include <string>
#include <sstream>
#include <trog/trog.hpp>

// if this line is not present this file will not compile as variable TrogFileLevel
// will not be defined.
// This line sets the logging fileter for this file
TROG_SET_FILE_LEVEL(Trog::LogLevelCTorTrace|Trog::LogLevelTrace3|Trog::LogLevelVerbose);

AClass::AClass()
{
    TROG_TRACE_CTOR();
}
std::string AClass::a_func(int i)
{
    TROG_DEBUG("The argument: ", i);
    TROG_TRACE3("TRACE3 this will display", i);
    TROG_TRACE4("TRACE4 this will not display", i);
    std::ostringstream os;

    os << __PRETTY_FUNCTION__ << " argument: " << i;
    
    std::string s = os.str();
    TROG_DEBUG("The return value : ", s);
    return s;
}
 AClass::~AClass()
{
    TROG_TRACE_CTOR();
}
