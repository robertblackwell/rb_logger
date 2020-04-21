//test_template.h:
#ifndef mytemplate_hpp
#define mytemplate_hpp
#include <cstdio>

template <class T>
class DemoT: T
{
public:

    void test();

};

#include "mytemplate.cpp"
#endif
