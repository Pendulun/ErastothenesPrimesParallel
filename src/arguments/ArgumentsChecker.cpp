#include "ArgumentsChecker.hpp"

namespace dataEntry{

    bool ArgumentsChecker::checkIfNumber(const char myString[]){
        std::stringstream ss(myString);
        std::string argument;

        while (std::getline(ss, argument, ' ')) {
            for(unsigned int idx = 0; idx < argument.size(); idx++){
                if(std::isdigit(argument.at(idx)) == 0){
                    return false;
                }
            }
        }

        

        return true;
    };

    bool ArgumentsChecker::setAllArguments(int argc, char const **argv){
        if(argc != NUM_ARGS){
            return false;
        }

        try{
            
            std::cout<<"Max number lido: "<<argv[1]<<"\n";
            if(!this->checkIfNumber(argv[1])){
                return false;
            }
            this->maxNumber = std::strtol(argv[1], NULL, 10);
            

            std::cout<<"Type lido: "<<argv[2]<<"\n";
            std::string type = (std::string) argv[2];
            if( type.compare("a") == 0 || 
                type.compare("l") == 0 || 
                type.compare("t") == 0) {
                    this->outType = type;
                }else{
                    return false;
                } 
            
            std::cout<<"NumProcessors lido: "<<argv[3]<<"\n";
            if(!this->checkIfNumber(argv[3])){
                return false;
            }
            this->numProcessors = std::strtol(argv[3], NULL ,10);

        }catch(const std::exception& e){
            return false;
        }

        return true;
    };

    unsigned int ArgumentsChecker::getNumProcessors(){
        return this->numProcessors;
    };

    unsigned int ArgumentsChecker::getMaxNumber(){
        return this->maxNumber;
    };

    std::string ArgumentsChecker::getOutType(){
        return this->outType;
    }

    bool ArgumentsChecker::isToPrintPrimes(){
        return this->outType == "l";
    }

    bool ArgumentsChecker::isToPrintTime(){
        return this->outType == "t";
    }

    bool ArgumentsChecker::isToPrintAllInfo(){
        return this->outType == "a";
    }
}

