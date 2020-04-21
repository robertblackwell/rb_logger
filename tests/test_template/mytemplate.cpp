//test_template.h:
#pragma once
#include <cstdio>
#include <iostream>
#include "mytemplate.hpp"

template <class T>
void DemoT<T>::test()
{
    std::cout << "ok inside DemoT::test this next bit is from T " << T::get_string() << std::endl;
}
