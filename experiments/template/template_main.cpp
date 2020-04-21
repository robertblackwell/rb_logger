#include <bitset>
#include <algorithm> 
#include <stdio.h>
#include <locale>
#include <chrono> 
#include <iostream> 
#include <memory>
#include <string>
#include <vector>
#include <string>
// #include <trog/collector.hpp>
// #include <trog/sink.hpp>
// #include <trog/writer.hpp>

#if 0
struct Opaque {
	int v1;
	int v2;
};



template<typename T>
void doSomething(std::vector<std::string>& v, T t)
{
	std::cout << "doSomething" << t << std::endl;
}
template<>
void doSomething(std::vector<std::string>& v, std::string s)
{
	std::cout << "doSomething" << s << std::endl;
	v.push_back(s);
}

template<>
void doSomething(std::vector<std::string>& v, Opaque op)
{
	std::cout << "doSomething v1 = " << op.v1 << " v2 = " << op.v2 << std::endl;
	std::string s = "Op: " + std::to_string(op.v1) + " " + std::to_string(op.v2);
	v.push_back(s);
}


/** Object that formats the message part of a LogCallData object during collections
 * and formats the entire logger output record in the backend.*/
class MyClass
{
    public:
	std::vector<std::string> m_vector;

    // void format_types(std::ostringstream& os);
    template <typename T, typename... Types>
    void format_types (const T& firstArg)
    {
	    doSomething(m_vector, firstArg);
    }

    template <typename T, typename... Types>
    void format_types (const T& firstArg, const Types&... args)
    {
        doSomething(m_vector, firstArg);
        format_types(args...);
    }
};
namespace Trog {
template<class W>
class Dummy
{
public:
	WriterInterfaceSPtr 			m_worker_sptr;
	FormatterSPtr					m_formatter_sptr;
	SinkInterfaceSPtr				m_sink_sptr;
	Dummy();
};
template<class W>
Dummy<W>::Dummy()
{
	// m_worker_sptr = std::make_shared<W>(m_formatter_sptr, m_sinks);
	m_worker_sptr = W::make(m_formatter_sptr, m_sinks);
}

typedef Dummy<Trog::Thread::Writer> ThreadDummy;
}

int main()
{
	Trog::ThreadDummy d{};
	std::string s("this  is a string");
	Opaque op1{1,1};
	Opaque op2 {2,2};
	MyClass m;
	std::cout << "hello";
}
#endif
#include "mytemplate.hpp"
#include "myclass.hpp"
typedef DemoT<MyClass> MyInstantiatedDemoClass;


template<typename A, typename B> struct C: public A, public B
{
	void test(){std::cout << "Class C " <<  A::me() << B::me() << std::endl;}
};

template<typename A, typename B, template<typename ...> class C>
struct D: public C<A, B>
{

	void test(){
		std::cout << "Class C " <<  A::m1me() << B::m2me() << std::endl;
	}
};

struct M1{
	std::string m1me(){ return std::string("This is M1::m1");}
};
struct M2{
	std::string m2me(){ return std::string("This is M2::m2");}
};



int main()
{
	D< M1, M2, C> xx;
	xx.test();
	DemoT<MyClass> x;
	x.test();
	MyInstantiatedDemoClass y;
	y.test();
}