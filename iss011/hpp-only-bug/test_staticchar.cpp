// Compile with g++ -Wall test_staticchar.cpp anotherclass.cpp
//
// > g++ test_staticchar.cpp anotherclass.cpp 
// /tmp/cc6hPNdu.o:(.rodata+0x0): multiple definition of `A<int>::str'
// /tmp/cc9onqew.o:(.rodata+0x0): first defined here
// collect2: error: ld returned 1 exit status
//


#include <iostream>
#include "staticchar.hpp"
#include "anotherclass.hpp"

int main()
{
  std::cout << A<int>::str << std::endl;
}
