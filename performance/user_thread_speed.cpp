#define TROG_ENABLE_FILE_LEVEL
#define TROG_ENABLE_GLOBAL_LEVEL
#include <trog/trog.hpp>

namespace Trog {
	typedef SinkFileT<SinkFileName> SinkIsAFile;
	using Trogger = Collector<Formatter, SinkIsAFile, Simple::Writer>;
};

TROG_SET_FILE_LEVEL(Trog::LogLevelVerbose);
TROG_SET_GLOBAL_LEVEL(Trog::LogLevelVerbose|Trog::LogLevelCTorTrace|Trog::LogLevelTrace3|Trog::LogLevelTrace4);
#include <bitset>
#include <algorithm> 
#include <stdio.h>
#include <locale>
#include <chrono> 
#include <iostream> 

 
 using namespace std::chrono;

void report(const char* fn_name, long d_total, long d_per_call)
{
	std::cout << std::endl << fn_name << " duration(nanosecs) total: " << d_total << " per call: " << d_per_call << std::endl;
}

void speed_test()
{
	std::string s1 = "Thisisstring1";
	char* s1_char_p = const_cast<char*>(s1.c_str());
	std::string s2 = "Thisisstring2";
	std::string s3 = "Thisisstring3";
	std::string s4 = "Thisisstring4";
	int number_iterations = 1000000;

	std::cout.imbue(std::locale(""));
	
	auto start = std::chrono::high_resolution_clock::now();
	for(int i = 0; i < number_iterations; i++) {
		TROG_ERROR("a warning");

	}
	TROG_INFO("we are done");
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = duration_cast<nanoseconds>(stop - start); 
	long d_total = duration.count();
	long d_per_call = d_total / number_iterations;

	std::cout << std::endl << "Duration(nanosecs) total: " << d_total << " per call: " << d_per_call << std::endl;
	report (__PRETTY_FUNCTION__, d_total, d_per_call);
}

int main()
{
	speed_test();
	std::cout << "leaving main" << std::endl;

}