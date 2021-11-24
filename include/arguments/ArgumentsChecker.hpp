#ifndef __ARGUMENTSCHECKER_H__
#define __ARGUMENTSCHECKER_H__
#include <string>
#include <exception>
#include <iostream>
#include <sstream>

#define NUM_ARGS 4

namespace dataEntry{
    class ArgumentsChecker{
        private:
            std::string outType;
            unsigned int numProcessors;
            unsigned int maxNumber;
            bool checkIfNumber(const char myString[]);
        public:
            bool setAllArguments(int argc, char const **argv);
            std::string getOutType();
            unsigned int getNumProcessors();
            unsigned int getMaxNumber();
            bool isToPrintPrimes();
            bool isToPrintTime();
            bool isToPrintAllInfo();
    } ;
}
#endif