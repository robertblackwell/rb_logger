///
/// Two things
/// -   #define TROG_FILE_LEVEL .... is required before the mpst of the TROG
///     header files are included in order to activiate the appropriate TROG
///     macros. If left out all the TROG macros will be disabled
/// -   It may (?) be helpful to have include <trog/level.hpp> included before the
///     #define as then the component macros such as TROG_LEVEL_DEBUG
///     will be available to your IDE's auto complete/intellisense
///
/// The macro TROG_FILE_LEVEL determines which TROG_ macro calls actually
/// generate code and collect logging data for possible output.
///
/// The final decision on what Trog outputs is make by the call to Trog::set_level()
/// in the main() function below
///

/// to disable Trog uncomment the following line
//#define TROG_DISABLE
#include <trog/loglevel.hpp>
#define TROG_FILE_LEVEL (TROG_LEVEL_VERBOSE | TROG_LEVEL_TRACE3 | TROG_LEVEL_CTOR )
#include "configure_trog.hpp"

#include "class_a.hpp"
#include "class_b.hpp"

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#pragma message "file lvel is : " STR(TROG_FILE_LEVEL)

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



int main()
{
    ///
    /// Trog::set_level() should be called at least once per executable. It filters
    /// the log data that is being collected to dertermine which data will be printed.
    ///
    /// This level may be changed during the running of a program.`
    ///
    Trog::set_level(TROG_LEVEL_TRACE3 | TROG_LEVEL_TRACE6 | TROG_LEVEL_VERBOSE);
    TROG_INFO("Entered main");
    TROG_VERBOSE("Iam verbose")
    TROG_TRACE3("from main")
    
    for(int i = 0; i < 10; i++) { 
        AClass a{};
        BClass b{};
        a.a_func(i);
        b.b_func(i);
    }
}