#ifndef __ERAS_SIEVE__
#define __ERAS_SIEVE__
#include <list>
#include <iostream>
#include <iterator>
#include <omp.h>
#include <math.h>
#include <chrono>

namespace primos{
    class ErastothenesSieve{
        public:
            void getPrimesTill(const unsigned int maxNumber, const unsigned int numProcs);
            void printAllPrimes();
            void printLastExecTime();
        private:
            std::list<unsigned int> numerosPrimos;
            std::chrono::duration<double, std::milli> timeExec;
            void getPrimesSequential1(const unsigned int maxNumber);
            void fillNSizeList(const unsigned int maxNumber);
            
            void getPrimesSequential2(const unsigned int maxNumber);
            bool isPrimeGivenList(const unsigned int number);

            void getPrimesParallel(const unsigned int maxNumber, const unsigned int numProcs);
            
            void dizOi();
    };
}

#endif