#include <iostream>
#include "ArgumentsChecker.hpp"
#include "ErastothenesSieve.hpp"

void usage(){
    std::cout<<"Invalid arguments!\n";
    std::cout<<"Usage: ./primes <max limit> <a|l|t> <num processors>\n";
    exit(1);
}

int main(int argc, const char **argv)
{
    dataEntry::ArgumentsChecker argumentsChecker;
    if(!argumentsChecker.setAllArguments(argc, argv)){
        usage();
    }

    primos::ErastothenesSieve erasSieve(argumentsChecker.getMaxNumber());

    unsigned int numMaxIteracoes = 10;
    double somaTempos = 0;
    for(int i = 0; i<numMaxIteracoes; i++){
        erasSieve.alocaEspacoVectorPrimos(argumentsChecker.getMaxNumber());
        erasSieve.getPrimesTill(argumentsChecker.getMaxNumber(), argumentsChecker.getNumProcessors());
        somaTempos += erasSieve.getExecTime();
        if(argumentsChecker.isToPrintPrimes() || argumentsChecker.isToPrintAllInfo()){
            erasSieve.printAllPrimes();
        }

        if(argumentsChecker.isToPrintTime() || argumentsChecker.isToPrintAllInfo()){
            erasSieve.printLastExecTime();
        }
    }

    double tempoMedio = somaTempos/numMaxIteracoes;

    std::cout<<"Tempo médio de execucao de "<<numMaxIteracoes<<" iteracoes foi de "<<tempoMedio<<std::endl;
    

    return 0;
}
