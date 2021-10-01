#include <trog/loglevel.hpp>
#define TROG_FILE_LEVEL TROG_LEVEL_VERBOSE
#include <trog/trog.hpp>

namespace Trog {

typedef SinkFileT<SinkFileName> SinkIsAFile;
typedef Sinkstdout SinkIsStdout;
typedef Sink2<Sinkstdout, SinkIsAFile> SinkIsAFileAndStdout;
// define our Trogger - uses dedicated task and rites to a file and cout

class Trogger : public Logger
{
    Formatter               m_fmter;
    SinkIsAFileAndStdout    m_sink;
    Thread::Writer          m_writer;// = new ::Trog::Simple::Writer(fmter, sink);
public:
    static Trogger& get_instance()
    {
        static Trogger instance;
        return instance;
    }
    Trogger(): m_writer(Thread::Writer(&m_fmter, &m_sink)), Logger(&m_writer, &m_fmter)
    {
        std::cout << "hello" << std::endl;
    }
};
inline Trogger& get_trogger_instance()
{
    return Trogger::get_instance();
}
}

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
	int number_iterations = 1000;//000;

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
    std::cout << "starting main" << std::endl;
    Trog::get_trogger_instance().set_threshold(TROG_LEVEL_ALL);
	speed_test();
	std::cout << "leaving main" << std::endl;

}