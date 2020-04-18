#include <bitset>
#include <algorithm> 
#include <stdio.h>
#include <locale>
#include <chrono> 
#include <iostream> 
#include <memory>
#include <string>
#include <vector>

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

template <class U>
class Y{
public:
    vector<U> v;
    template <class T>
    Y(T n) {
        v.push_back(n);
    }
    template <class T, class... T2>
    Y(T n, T2... rest) {
        v.push_back(n);
        Y(rest...);
    }
};

template <class T>
struct X
{
    std::vector<T> v;
    X() = default; //Terminating recursion

    template <class U, class... Ts>
    X(U n, Ts... rest)  : X(rest...) { .. the recursive work ..}
};


int main()
{
	std::string s("this  is a string");
	Opaque op1{1,1};
	Opaque op2 {2,2};
	MyClass m;
	m.format_types(s, op1, op2);
	Y<int> y{1,2,3,4,5};
	X<int> x{1,2,3,4,5,6};
	std::cout << "hello";
}