#include "ErastothenesSieve.hpp"

namespace primos{
    void ErastothenesSieve::getPrimesTill(const unsigned int maxNumber){
        //Crie uma lista de inteiros consecutivos de 2 até maxNumber
        for(unsigned int num = 2; num<=maxNumber; num++){
            this->numerosPrimos.push_back(num);
        }
        //Seja p=2 o primeiro número primo
        unsigned int primeAtual = 0;
        unsigned int primeCount = 0;

        std::list<unsigned int>::iterator iterador;

        auto initTime = std::chrono::high_resolution_clock::now();

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

        auto endTime = std::chrono::high_resolution_clock::now();
        this->timeExec = std::chrono::duration_cast<std::chrono::microseconds>(endTime - initTime);
        //Encontre o primeiro número na lista que seja maior do que p e que não esteja marcado.
        //Caso não exista, pare. Se existir, p vai ser igual a esse novo número.

        //Repita a partir do passo 3

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

    void ErastothenesSieve::printAllPrimes(){
        std::list<unsigned int>::iterator iterador = this->numerosPrimos.begin();
        for(iterador; iterador != this->numerosPrimos.end(); iterador++){
            std::cout<<*iterador<<" ";
        }

        std::cout<<std::endl;
    }

    void ErastothenesSieve::printLastExecTime(){
        std::cout<<this->timeExec.count()<<std::endl;
    }

    void ErastothenesSieve::dizOi(){
        std::cout<< "Oi da Thread: "<< omp_get_thread_num()<< " de "<<omp_get_max_threads()<<"\n";
    }
}