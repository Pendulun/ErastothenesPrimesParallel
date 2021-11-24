#ifndef __ERAS_SIEVE__
#define __ERAS_SIEVE__
#include <list>
#include <iostream>
#include <iterator>
#include <omp.h>
#include <chrono>

namespace primos{
    class ErastothenesSieve{
        public:
            void getPrimesTill(const unsigned int maxNumber);
            void printAllPrimes();
            void printLastExecTime();
        private:
            std::list<unsigned int> numerosPrimos;
            std::chrono::duration<double> timeExec;
            void dizOi();
    };
}

#endif