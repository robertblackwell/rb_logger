#include <bitset>
#include <algorithm> 
#include <stdio.h>
#include <locale>
#include <chrono> 
#include <iostream> 
#include <trog/trog.hpp>
 
 using namespace std::chrono;

void report(const char* fn_name, long d_total, long d_per_call)
{
	std::cout << std::endl << fn_name << " duration(nanosecs) total: " << d_total << " per call: " << d_per_call << std::endl;
}

void speed_test_logWithFormat()
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
		Trog::Trogger::activeTrogger.logWithFormat(
			Trog::LogLevelError, Trog::LogLevelInfo,
			__FILE__, __PRETTY_FUNCTION__, __LINE__,
			"Format %s %s %s \n", s2.c_str(), s3.c_str(), s4.c_str()
		);
	}
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = duration_cast<nanoseconds>(stop - start); 
	long d_total = duration.count();
	long d_per_call = d_total / number_iterations;

	std::cout << std::endl << "Duration(nanosecs) total: " << d_total << " per call: " << d_per_call << std::endl;
	report (__PRETTY_FUNCTION__, d_total, d_per_call);
}
void speed_test_traceLog()
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
		Trog::Trogger::activeTrogger.tracelog(
			Trog::LogLevelError, Trog::LogLevelInfo,
			__FILE__, __PRETTY_FUNCTION__, __LINE__,
			s2, s2, s2, s2, s2, s2, s2, s2, s2, s2
		);
	}
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = duration_cast<nanoseconds>(stop - start); 
	long d_total = duration.count();
	long d_per_call = d_total / number_iterations;

	std::cout << std::endl << "Duration(nanosecs) total: " << d_total << " per call: " << d_per_call << std::endl;
	report (__PRETTY_FUNCTION__, d_total, d_per_call);

}
void speed_test_myprint()
{
	std::ostringstream os;
	std::string s1 = "Thisisstring1";
	char* s1_char_p = const_cast<char*>(s1.c_str());
	std::string s2 = "Thisisstring2";
	std::string s3 = "Thisisstring3";
	std::string s4 = "Thisisstring4";
	int number_iterations = 1000000;

	std::cout.imbue(std::locale(""));
	
	auto start = std::chrono::high_resolution_clock::now();
	for(int i = 0; i < number_iterations; i++) {
		// std::string file_name = __FILE__;
		// boost::filesystem::path file_path = boost::filesystem::path(file_name);

		boost::filesystem::path file_path{__FILE__};
	    auto tmp3 = file_path.filename();
    	// auto tmp4 = tmp3.stem();

		Trog::preamble(os, __FILE__, 42, 42, __PRETTY_FUNCTION__, __LINE__);
		Trog::Trogger::activeTrogger.myprint(os, s2, s2, s2, s2, s2, s2, s2, s2, s2, s2);
	}
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = duration_cast<nanoseconds>(stop - start); 
	long d_total = duration.count();
	long d_per_call = d_total / number_iterations;

	std::cout << std::endl << "Duration(nanosecs) total: " << d_total << " per call: " << d_per_call << std::endl;
	report (__PRETTY_FUNCTION__, d_total, d_per_call);

}
void speed_test_collect()
{
	std::ostringstream os;
	std::string s1 = "Thisisstring1";
	char* s1_p = const_cast<char*>(s1.c_str());
	std::string s2 = "Thisisstring2";
	std::string s3 = "Thisisstring3";
	std::string s4 = "Thisisstring4";
	int number_iterations = 1000000;

	std::cout.imbue(std::locale(""));
	
	auto start = std::chrono::high_resolution_clock::now();
	for(int i = 0; i < number_iterations; i++) {
		Trog::collect(Trog::LogLevelError, Trog::LogLevelError, "CHANNEL", __FILE__, __PRETTY_FUNCTION__, __LINE__, s2, s2, s2, s2, s2, s2, s2, s2, s2, s2);
	}
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = duration_cast<nanoseconds>(stop - start); 
	long d_total = duration.count();
	long d_per_call = d_total / number_iterations;

	std::cout << std::endl << "Duration(nanosecs) total: " << d_total << " per call: " << d_per_call << std::endl;
	report (__PRETTY_FUNCTION__, d_total, d_per_call);

}
void speed_test_asprintf()
{
	std::ostringstream os;
	std::string s1 = "Thisisstring1";
	char* s1_p = const_cast<char*>(s1.c_str());
	std::string s2 = "Thisisstring2";
	std::string s3 = "Thisisstring3";
	std::string s4 = "Thisisstring4";
	int number_iterations = 1000000;

	std::cout.imbue(std::locale(""));
	
	auto start = std::chrono::high_resolution_clock::now();
	for(int i = 0; i < number_iterations; i++) {
		char* vp = nullptr;
		long pid = 42; 
		long tid=42;
		asprintf(&vp, "file: %s pid: %ld tid: %ld func: %s line: %d %s1 %s 2 %s 3 %s 4 %s 5 %s 6 %s 7 %s 8 %s 9 %s A %s \n", 
			__FILE__,
			pid, tid,
			__PRETTY_FUNCTION__,
			__LINE__,

			s1_p,
			s1_p,
			s1_p,
			s1_p,
			s1_p,
			s1_p,
			s1_p,
			s1_p,
			s1_p,
			s1_p);
		free(vp);
	}
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = duration_cast<nanoseconds>(stop - start); 
	long d_total = duration.count();
	long d_per_call = d_total / number_iterations;

	std::cout << std::endl << "Duration(nanosecs) total: " << d_total << " per call: " << d_per_call << std::endl;
	report (__PRETTY_FUNCTION__, d_total, d_per_call);

}

int main()
{
	speed_test_traceLog();
	speed_test_myprint();
	speed_test_asprintf();
}