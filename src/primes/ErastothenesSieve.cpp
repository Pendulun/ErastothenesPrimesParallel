#include "ErastothenesSieve.hpp"

namespace primos{

    ErastothenesSieve::ErastothenesSieve(unsigned int maxNumber){
        this->alocaEspacoVectorPrimos(maxNumber);
    };

    void ErastothenesSieve::alocaEspacoVectorPrimos(const unsigned int maxNumber){
        this->numerosPrimosVector.clear();
        //By https://primes.utm.edu/howmany.html
        unsigned int initSize = floor(maxNumber/(log(maxNumber) - 1));
        this->numerosPrimosVector.reserve(initSize);
    }

    void ErastothenesSieve::getPrimesTill(const unsigned int maxNumber, const unsigned int numProcs){

        auto initTime = std::chrono::high_resolution_clock::now();
        if(numProcs == 1){
            this->getPrimesSequential(maxNumber);
        }else{
            this->getPrimesParallelVector(maxNumber, numProcs);
        }
        auto endTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> fp_ms = endTime - initTime;
        this->timeExec = std::chrono::duration_cast<std::chrono::milliseconds>(fp_ms);
    }

    void ErastothenesSieve::getPrimesSequential(const unsigned int maxNumber){
        
        this->numerosPrimosVector.clear();

        if(maxNumber<2){
            return;
        }

        this->numerosPrimosVector.push_back(2);

        if(maxNumber == 2){
            return;
        }

        unsigned int maxLimitForLoop = 0;
        if(maxNumber%2 == 0){
            maxLimitForLoop = (maxNumber/2) - 1;
        }else{
            maxLimitForLoop = floor(maxNumber/2);
        }

        unsigned int numAtual = 0;
        for(unsigned int num = 1; num<=maxLimitForLoop; num++){
            numAtual = (2*num)+1;
            if(this->isPrimeGivenVectorSequential(numAtual)){
                this->numerosPrimosVector.push_back(numAtual);
            }
        }

    }

    bool ErastothenesSieve::isPrimeGivenVectorSequential(const unsigned int number){
        long double squared_number = sqrt(number);
        bool result = false;
        for(std::size_t idxPrime = 0; idxPrime < this->numerosPrimosVector.size(); idxPrime++){
            unsigned int element = this->numerosPrimosVector[idxPrime];
            if(element > squared_number){
                result = true;
                break;
            }

            if((number%element) == 0){
                result = false;
                break;
            }
        }

        return result;
    }

    void ErastothenesSieve::getPrimesParallelVector(const unsigned int maxNumber, const unsigned int numProcs){
        this->numerosPrimosVector.clear();

        if(maxNumber<2){
            return;
        }

        this->numerosPrimosVector.push_back(2);

        if(maxNumber == 2){
            return;
        }

        unsigned int maxLimitForLoop = 0;
        if(maxNumber%2 == 0){
            maxLimitForLoop = (maxNumber/2) - 1;
        }else{
            maxLimitForLoop = floor(maxNumber/2);
        }

        unsigned int numAtual = 0;
        for(unsigned int num = 1; num<=maxLimitForLoop; num++){
            numAtual = (2*num)+1;

            bool isPrime = this->isPrimeGivenVectorParallel(numAtual, numProcs);
            if(isPrime){
                this->numerosPrimosVector.push_back(numAtual);
            }
        }
    }

    bool ErastothenesSieve::isPrimeGivenVectorParallel(const unsigned int number, const unsigned int numProcs){
        unsigned int squared_number = ceil(sqrt(number));
        bool globalNotPrime = false;
        #pragma omp parallel num_threads(numProcs) 
        {
            bool localNotPrime = false;
            #pragma omp for nowait schedule(auto)
            for(unsigned int divisor = 2; divisor <= squared_number; divisor++){
                    localNotPrime = this->isMultipleOf(number, divisor) || localNotPrime;
            }

            #pragma omp critical
            {
                globalNotPrime = globalNotPrime || localNotPrime;  
            }   
        }
        return !globalNotPrime;
    }


    bool ErastothenesSieve::isMultipleOf(const unsigned int num, const unsigned int prime){
        
        if((num%prime) == 0){
            return true;
        }else{
            return false;
        }
    }

    void ErastothenesSieve::printAllPrimes(){
        if(this->numerosPrimosVector.size() > 0){
            unsigned int primesPrinted = 0;
            std::vector<unsigned int>::iterator iterador = this->numerosPrimosVector.begin();
            for(iterador; iterador != this->numerosPrimosVector.end(); iterador++){
                std::cout<<*iterador;
                if(primesPrinted == this->numerosPrimosVector.size()-1){
                    std::cout<<std::endl;
                }else{
                    std::cout<<" ";
                }
                primesPrinted++;
            }
        }
    }

    void ErastothenesSieve::clearPrimes(){
        this->numerosPrimosVector.clear();
        this->numerosPrimosVector.shrink_to_fit();
    }

    void ErastothenesSieve::printLastExecTime(){
        std::cout<<this->timeExec.count()<<std::endl;
    }

    double ErastothenesSieve::getExecTime(){
        return this->timeExec.count();
    }

}