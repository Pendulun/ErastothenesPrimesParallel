#ifndef __ERAS_SIEVE__
#define __ERAS_SIEVE__
#include <list>
#include <iostream>
#include <iterator>
#include <omp.h>

namespace primos{
    class ErastothenesSieve{
        public:
            void getPrimesTill(const unsigned int maxNumber);
        private:
        std::list<unsigned int> numerosPrimos;
        void dizOi();
    };
}

#endif