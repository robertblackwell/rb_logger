#include <trog/trog.hpp>
#include "class_a.hpp"
#include "class_b.hpp"
#include "configure_trog.hpp"

TROG_SET_FILE_LEVEL(Trog::LogLevelVerbose);

///
/// This macro needs to be called in exactly one compilation unit.
/// If omitted you will get a linker error (and maybe a compile error) complaining that
/// variable TrogGlobalLevel is undefined.
///
/// If you call it in more than one compilation unit you will get a multily defined symbol error
/// for the same variable
///
/// This macro sets the global filter - here we are allowing all levels up to an including Verbose
/// plus allowing LogLevelCToreTrace as welss a Trace3 and Trace4
///
TROG_SET_GLOBAL_LEVEL(Trog::LogLevelVerbose|Trog::LogLevelCTorTrace|Trog::LogLevelTrace3|Trog::LogLevelTrace4);

int main()
{
    TROG_INFO("Entered main");
    
    for(int i = 0; i < 10; i++) {
        AClass a{};
        BClass b{};
        a.a_func(i);
        b.b_func(i);
    }
}