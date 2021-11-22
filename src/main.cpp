#include <iostream>
#include "ArgumentsChecker.hpp"

void usage(){
    std::cout<<"Invalid arguments!\n";
    std::cout<<"Usage: ./primes <max limit> <a|l|t> <num processors>\n";
    exit(1);
}

int main(int argc, const char **argv)
{
    dataEntry::ArgumentsChecker argumentsChecker;
    if(!argumentsChecker.setAllArguments(argc, argv)){
        usage();
    }

    return 0;
}
