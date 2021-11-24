#include <iostream>
#include "ArgumentsChecker.hpp"
#include "ErastothenesSieve.hpp"

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

    primos::ErastothenesSieve erasSieve;

    erasSieve.getPrimesTill(argumentsChecker.getMaxNumber());
    if(argumentsChecker.isToPrintPrimes() || argumentsChecker.isToPrintAllInfo()){
        erasSieve.printAllPrimes();
    }

    if(argumentsChecker.isToPrintTime() || argumentsChecker.isToPrintAllInfo()){
        erasSieve.printLastExecTime();
    }
    //Tratar caso maxNumber for <= 2
    return 0;
}
