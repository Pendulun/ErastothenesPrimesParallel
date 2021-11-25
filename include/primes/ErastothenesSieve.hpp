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
            void getPrimesSequential(const unsigned int maxNumber);
            void getPrimesParallel(const unsigned int maxNumber, const unsigned int numProcs);
            void fillNSizeList(const unsigned int maxNumber);
            void dizOi();
    };
}

#endif