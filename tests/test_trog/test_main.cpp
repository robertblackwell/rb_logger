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
#include <trog/loglevel.hpp>
#define TROG_FILE_LEVEL (TROG_LEVEL_DEBUG | TROG_LEVEL_TRACE3 | TROG_LEVEL_CTOR )
#include "configure_trog.hpp"

#include <bitset>
#include <doctest/doctest.h>
#define DOCTEST_CONFIG_IMPLEMENT
#include <boost/filesystem.hpp>


int main()
{

//	displayBitMask();
//	testLevelText();
	TROG_ERROR("This is an error");
	TROG_WARN("This is an warning");
	TROG_INFO("This is info");
	TROG_DEBUG("This is debug", "and this is some more", 42);
	TROG_VERBOSE("This is verbose");
//	ATestClass* aptr = new ATestClass();
//	aptr->doSomething();
//	ATestClass* bptr = new ATestClass();
//	delete aptr;
//	delete bptr;
	TROG_TRACE_FD(33);
	TROG_TRACE3("This is just a trace");
	TROG_ERROR("This is an error");
//	testBitMask();
}