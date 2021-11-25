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

    void ErastothenesSieve::getPrimesTill(const unsigned int maxNumber, const unsigned int numProcs){

        auto initTime = std::chrono::high_resolution_clock::now();
        if(numProcs == 1){
            this->getPrimesSequential2Vector(maxNumber);
        }else{
            this->getPrimesParallel(maxNumber, numProcs);
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
            if(this->isPrimeGivenVector(numAtual)){
                this->numerosPrimosVector.push_back(numAtual);
            }
        }

    }

     bool ErastothenesSieve::isPrimeGivenVector(const unsigned int number){
        long double squared_number = sqrt(number);
        //Dá para paralelizar isso
        bool result = false;
        bool definiuResult = false;
        for(std::size_t idxPrime = 0; idxPrime < this->numerosPrimosVector.size(); idxPrime++){
            unsigned int element = this->numerosPrimosVector[idxPrime];
            if(!definiuResult){
                if(element > squared_number){
                    result = true;
                    definiuResult = true;
                }
            
                if((number%element) == 0){
                    result = false;
                    definiuResult = true;
                }
            } 
        }

        return result;

    }

    void ErastothenesSieve::getPrimesParallel(const unsigned int maxNumber, const unsigned int numProcs){
        //Retorne os números não marcados
        // #pragma omp parallel num_threads(8)
        // #pragma omp single
        // {
        //     for(unsigned int i = 0; i<maxNumber; i++){
        //         #pragma omp task
        //         this->dizOi();
        //     }
        // std::cout<<"Saiu\n";
        // #pragma omp taskwait
        // }   
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