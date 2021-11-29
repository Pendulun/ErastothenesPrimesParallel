#include "ErastothenesSieve.hpp"

namespace primos{

    ErastothenesSieve::ErastothenesSieve(unsigned int maxNumber){
        this->alocaEspacoVectorPrimos(maxNumber);
    };

    void ErastothenesSieve::alocaEspacoVectorPrimos(const unsigned int maxNumber){
        this->numerosPrimosVector.clear();
        //By https://primes.utm.edu/howmany.html
        unsigned int initSize = floor(maxNumber/(log(maxNumber) - 1));
        std::cout<<"Tam inicial alocado: "<<initSize<<"\n";
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
            bool isPrime = this->isPrimeGivenVectorParallel3(numAtual, numProcs);

            //std::cout<<numAtual<<" ISPRIME: "<<isPrime<<std::endl;
            if(isPrime){
                this->numerosPrimosVector.push_back(numAtual);
            }
        }
    }

    bool ErastothenesSieve::isPrimeGivenVectorParallel1(const unsigned int number, const unsigned int numProcs){
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

    bool ErastothenesSieve::isPrimeGivenVectorParallel2(const unsigned int number, const unsigned int numProcs){
        long double squared_number = sqrt(number);
        unsigned int divisor;
        unsigned int logBase = 2;
        //log_{b}^{a} = log(a)/log(b)
        if(this->numerosPrimosVector.size() > logBase){
            long double resultBase = log(this->numerosPrimosVector.size())/log(logBase);
            //std::cout<<"resultBase: "<<resultBase<<std::endl;
            divisor = floor(resultBase*2);
        }else{
            divisor = 1;
        }
        

        bool globalNotPrime = false;
        bool foundIsMultiple = false;
        bool primeBiggerThanSqrt = false;
        bool checkedAllPrimes = false;
        unsigned int blockCount = 0;
        unsigned int blockSize = 1;
        if(this->numerosPrimosVector.size() > divisor){
            blockSize = ceil(this->numerosPrimosVector.size()/divisor);
        }
        
        //std::cout<<"BLock SIze: "<<blockSize<<std::endl;
         
        while(!globalNotPrime && !primeBiggerThanSqrt && !checkedAllPrimes){
            bool localNotPrime = false;
            unsigned int blockStartIndex = blockSize*blockCount;
            //std::cout<<"Block start: "<<blockStartIndex;
            unsigned int blockEndIndex = blockStartIndex+blockSize;
            //std::cout<<" Block end: "<<blockEndIndex;
            if(blockEndIndex >= this->numerosPrimosVector.size()-1){
                blockEndIndex = this->numerosPrimosVector.size();
                checkedAllPrimes = true;
            }
            //std::cout<<" Checking all primes: "<<checkedAllPrimes<<std::endl;
            #pragma omp parallel num_threads(numProcs) \
                default(none) \ 
                shared(squared_number, globalNotPrime, primeBiggerThanSqrt, blockStartIndex, blockEndIndex, logBase) \
                firstprivate(localNotPrime)
            {
            #pragma omp for nowait schedule(static, 2*logBase)
            for(std::size_t idxPrime = blockStartIndex; idxPrime < blockEndIndex; idxPrime++){
                unsigned int elemento = this->numerosPrimosVector[idxPrime];
                if(elemento <= squared_number){
                    localNotPrime = this->isMultipleOf(number, elemento) || localNotPrime;
                }else{
                    #pragma omp critical(endPrimeSearchSqrt) 
                    primeBiggerThanSqrt = true;
                }
                
            }

            #pragma omp critical(verifyNotPrime)
                globalNotPrime = globalNotPrime || localNotPrime;
            }

            blockCount++;

        }
        
        return !globalNotPrime;

    }

    bool ErastothenesSieve::isPrimeGivenVectorParallel3(const unsigned int number, const unsigned int numProcs){
        unsigned int squared_number = ceil(sqrt(number));
        bool globalNotPrime = false;
        #pragma omp parallel num_threads(numProcs) 
        {
            bool localNotPrime = false;
            #pragma omp for nowait schedule(static, 2)
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

    bool ErastothenesSieve::isPrimeGivenVectorParallel4(const unsigned int number, const unsigned int numProcs){
        unsigned int root_number = ceil(sqrt(number));
        unsigned int numBlocks = floor(log(root_number));

        //Se não vale a pena dividir mais de um bloco
        if(root_number < 40){
            numBlocks = 1;
        }

        unsigned int blockSize = ceil(root_number/numBlocks);
        
        unsigned int blockCount = 0;
        bool globalIsPrime = true;
        bool checkedAllPrimes = false;
        while(globalIsPrime  && !checkedAllPrimes){
            unsigned int blockStartNum = blockSize*blockCount;
            
            unsigned int blockEndNum = blockStartNum+blockSize;

            //Não precisa checar se é múltiplo de 0 ou 1
            if(blockStartNum == 0){
                blockStartNum = 2;
            }

            //Não precisamos verificar além da raíz do número
            if(blockEndNum > root_number){
                blockEndNum = root_number;
            }

            #pragma omp parallel num_threads(numProcs) \
                default(none) \ 
                shared(root_number, globalIsPrime, blockStartNum, blockEndNum)
                {
                    bool localIsPrime = true;
                #pragma omp for nowait schedule(static, 1) reduction(&&: globalIsPrime)
                    for(unsigned int numDivisor = blockStartNum; numDivisor <= blockEndNum; numDivisor++){
                        localIsPrime = !this->isMultipleOf(number, numDivisor) && localIsPrime;
                    }

                    //Se alguma Thread retornar que não é primo, globalIsPrime é definido como falso
                    #pragma omp critical
                    globalIsPrime = globalIsPrime && localIsPrime;
                }
            
            
            if(blockCount == numBlocks){
                checkedAllPrimes = true;
            }

            blockCount++;
            
        }
        
        return globalIsPrime;
    }

    bool ErastothenesSieve::isMultipleOf(const unsigned int num, const unsigned int prime){
        //std::cout<<"Thread "<<omp_get_thread_num()<<" se "<<num<<" e multiplo de "<<prime<<std::endl;
        
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
        // std::cout<<"QTD PRIMES GENERATED: "<<this->numerosPrimos.size()<<std::endl;
        std::cout<<"QTD PRIMES GENERATED: "<<this->numerosPrimosVector.size()<<std::endl;
    }

    double ErastothenesSieve::getExecTime(){
        return this->timeExec.count();
    }

}