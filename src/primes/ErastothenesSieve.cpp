#include "ErastothenesSieve.hpp"

namespace primos{

    ErastothenesSieve::ErastothenesSieve(unsigned int maxNumber){
        this->numerosPrimosVector.clear();
        //By https://primes.utm.edu/howmany.html
        unsigned int initSize = floor(maxNumber/(log(maxNumber) - 1));
        std::cout<<"Tam inicial alocado: "<<initSize<<"\n";
        this->numerosPrimosVector.reserve(initSize);
        //this->numerosPrimosVector.shrink_to_fit();
    };

    void ErastothenesSieve::clearPrimes(){
        this->numerosPrimosVector.clear();
        this->numerosPrimosVector.shrink_to_fit();
    }

    void ErastothenesSieve::getPrimesTill(const unsigned int maxNumber, const unsigned int numProcs){

        auto initTime = std::chrono::high_resolution_clock::now();
        if(numProcs == 1){
            this->getPrimesSequential2Vector(maxNumber);
        }else{
            this->getPrimesParallelVector(maxNumber, numProcs);
        }
        auto endTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> fp_ms = endTime - initTime;
        this->timeExec = std::chrono::duration_cast<std::chrono::milliseconds>(fp_ms);
    }

    void ErastothenesSieve::getPrimesSequential1(const unsigned int maxNumber){
        
        this->fillNSizeList(maxNumber);

        unsigned int primeAtual = 0;
        unsigned int primeCount = 0;

        std::list<unsigned int>::iterator iterador;
        while(true){
            //Get the next prime
            if(primeCount == this->numerosPrimos.size()){
                break;
            }


            iterador = std::next(this->numerosPrimos.begin(), primeCount);
            primeAtual = *iterador;

            //Start search begining with the number next to the last prime identified
            iterador = this->numerosPrimos.begin();
            std::advance(iterador, primeCount+1);


            for(iterador; iterador!=numerosPrimos.end();iterador++){
                //Remove number if it's not a prime
                if(*iterador != primeAtual && *iterador%primeAtual == 0){
                    iterador = this->numerosPrimos.erase(iterador);
                }
            }

            primeCount += 1;
        }
    }

    void ErastothenesSieve::getPrimesSequential2(const unsigned int maxNumber){
        this->numerosPrimos.clear();
        if(maxNumber<2){
            return;
        }

        this->numerosPrimos.push_back(2);
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
            if(this->isPrimeGivenList(numAtual)){
                this->numerosPrimos.push_back(numAtual);
            }
        }
    }

    bool ErastothenesSieve::isPrimeGivenList(const unsigned int number){
        long double squared_number = sqrt(number);
        //Dá para paralelizar isso
        for(auto iterador = this->numerosPrimos.begin(); *iterador <= squared_number; iterador++){
            if(number%(*iterador) == 0){
                return false;
            }
        }

        return true;


        // for(auto prime : this->numerosPrimos){
        //     if(prime > squared_number){
        //         return true;
        //     }

        //     if(number%prime == 0){
        //         return false;
        //     }
        // }
    }

    void ErastothenesSieve::getPrimesSequential2Vector(const unsigned int maxNumber){
        
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
        //Dá para paralelizar isso ?
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

            //std::cout<<num<<" ISPRIME: "<<isPrime<<std::endl;
            if(isPrime){
                this->numerosPrimosVector.push_back(numAtual);
            }
        }
    }

    bool ErastothenesSieve::isPrimeGivenVectorParallel(const unsigned int number, const unsigned int numProcs){
        long double squared_number = sqrt(number);
        bool globalNotPrime = false;
        unsigned int maxIdx = this->numerosPrimosVector.size();
        #pragma omp parallel num_threads(numProcs) 
        {
            bool localNotPrime = false;
            #pragma omp for nowait schedule(static, 2)
            for(std::size_t idxPrime = 0; idxPrime < maxIdx; idxPrime++){
                unsigned int elemento = this->numerosPrimosVector[idxPrime];
                if(elemento <= squared_number){
                    localNotPrime = this->isMultipleOf(number, elemento) || localNotPrime;
                }
                
            }

            #pragma omp atomic
            globalNotPrime = globalNotPrime | localNotPrime;

        }
        
        return !globalNotPrime;

    }

    bool ErastothenesSieve::isMultipleOf(const unsigned int num, const unsigned int prime){
        //std::cout<<"Thread "<<omp_get_thread_num()<<" se "<<num<<" e multiplo de "<<prime<<std::endl;
        
        if((num%prime) == 0){
            return true;
        }else{
            return false;
        }
    }


    void ErastothenesSieve::fillNSizeList(const unsigned int maxNumber){
        this->numerosPrimos.clear();
        if(maxNumber<2){
            return;
        }

        this->numerosPrimos.push_back(2);
        if(maxNumber == 2){
            return;
        }

        unsigned int maxLimitForLoop = 0;
        if(maxNumber%2 == 0){
            maxLimitForLoop = (maxNumber/2) - 1;
        }else{
            maxLimitForLoop = floor(maxNumber/2);
        }

        for(unsigned int num = 1; num<=maxLimitForLoop; num++){
            this->numerosPrimos.push_back((2*num)+1);
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

    void ErastothenesSieve::printLastExecTime(){
        std::cout<<this->timeExec.count()<<std::endl;
        // std::cout<<"QTD PRIMES GENERATED: "<<this->numerosPrimos.size()<<std::endl;
        std::cout<<"QTD PRIMES GENERATED: "<<this->numerosPrimosVector.size()<<std::endl;
    }

    void ErastothenesSieve::dizOi(){
        std::cout<< "Oi da Thread: "<< omp_get_thread_num()<< " de "<<omp_get_max_threads()<<"\n";
    }
}