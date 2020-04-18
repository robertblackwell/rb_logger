#include <bitset>
#include <string>
#include <iostream>
#include <exception>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <memory>
void ouch(int sig)
{
    printf("OUCH! - I got signal %d\n", sig);
}
// void setup_sighandler()
// {
//  	struct sigaction act;
//     act.sa_handler = ouch;
//     sigemptyset(&act.sa_mask);
//     act.sa_flags = 0;
//     sigaction(SIGINT, &act, 0);
// }

class Worker
{
	public:
	Worker()
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
	void cleanup()
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
	~Worker()
	{
		cleanup();
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
};

std::shared_ptr<Worker> wsptr;
void exit_handler()
{
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	wsptr->cleanup();	
	std::exit(0);
}
void unexpected_handler()
{
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	wsptr->cleanup();	
	std::exit(1);
}
void terminate_handler()
{
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	wsptr->cleanup();	
	std::cout << "terminating" << std::endl;
	std::abort;
}

int main()
{
	wsptr = std::make_shared<Worker>();
	std::set_unexpected(unexpected_handler);
	std::set_terminate(terminate_handler);
	std::atexit(exit_handler);

std::cout << "entered main" << std::endl;
#if TEST_SIG
try{
	char* p = nullptr;
	int q = 0;
	int xx = 72 / q;
	char x = p[3];
} catch(...){
	std::cout << "catch exception" << std::endl;
}
#elif TEST_UNCAGHT_EXCEPTION
	throw "an exception inside main";
#else
	try {
		throw "an exception inside main";
	} catch(std::exception e){
		std::cout << "catch exception" << std::endl;
	}
#endif
std::cout << "leaving main" << std::endl;
}