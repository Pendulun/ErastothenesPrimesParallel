#ifndef __ERAS_SIEVE__
#define __ERAS_SIEVE__
#include <list>
#include <vector>
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
            void clearPrimes();
            double getExecTime();
            void alocaEspacoVectorPrimos(const unsigned int N);
            ErastothenesSieve(unsigned int maxNumber);
        private:
            std::chrono::duration<double, std::milli> timeExec;
            std::vector<unsigned int> numerosPrimosVector;

            void getPrimesSequential(const unsigned int maxNumber);
            bool isPrimeGivenVectorSequential(const unsigned int number);

            void getPrimesParallel(const unsigned int maxNumber, const unsigned int numProcs);
            void getPrimesParallelVector(const unsigned int maxNumber, const unsigned int numProcs);
            bool isPrimeGivenVectorParallel3(const unsigned int number, const unsigned int numProcs);
            bool isPrimeGivenVectorParallel4(const unsigned int number, const unsigned int numProcs);
            bool isMultipleOf(const unsigned int num, const unsigned int prime);
    };
}

#endif