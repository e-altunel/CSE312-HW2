#include <iostream>

int
main() {
#ifndef NDEBUG
	std::cout << "Debug mode" << std::endl;
#else
	std::cout << "Release mode" << std::endl;
#endif
}