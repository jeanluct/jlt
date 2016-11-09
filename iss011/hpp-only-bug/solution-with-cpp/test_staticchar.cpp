// Compile with g++ -Wall test_staticchar.cpp anotherclass.cpp staticchar.cpp

#include <iostream>
#include "staticchar.hpp"
#include "anotherclass.hpp"

int main()
{
  std::cout << A<int>::str << std::endl;
}
