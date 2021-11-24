#include "ErastothenesSieve.hpp"

namespace primos{
    void ErastothenesSieve::getPrimesTill(const unsigned int maxNumber, const unsigned int numProcs){

        this->fillNSizeList(maxNumber);

        auto initTime = std::chrono::high_resolution_clock::now();
        if(numProcs == 1){
            this->getPrimesSequential(maxNumber);
        }else{
            this->getPrimesParallel(maxNumber, numProcs);
        }
        auto endTime = std::chrono::high_resolution_clock::now();
        this->timeExec = std::chrono::duration_cast<std::chrono::seconds>(endTime - initTime);

    }

    void ErastothenesSieve::getPrimesSequential(const unsigned int maxNumber){
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
                if(*iterador%primeAtual == 0){
                    iterador = this->numerosPrimos.erase(iterador);
                }
            }

            primeCount += 1;
        }
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
        for(unsigned int num = 2; num<=maxNumber; num++){
            this->numerosPrimos.push_back(num);
        }
    }

    void ErastothenesSieve::printAllPrimes(){
        if(this->numerosPrimos.size() > 0){
            unsigned int primesPrinted = 0;
            std::list<unsigned int>::iterator iterador = this->numerosPrimos.begin();
            for(iterador; iterador != this->numerosPrimos.end(); iterador++){
                std::cout<<*iterador;
                if(primesPrinted == this->numerosPrimos.size()-1){
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
    }

    void ErastothenesSieve::dizOi(){
        std::cout<< "Oi da Thread: "<< omp_get_thread_num()<< " de "<<omp_get_max_threads()<<"\n";
    }
}