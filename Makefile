CC=g++ # compilador, troque para gcc se preferir utilizar C
CFLAGS=-Wall -g #-Wextra # compiler flags, troque o que quiser, exceto bibliotecas externas
EXEC=./primes # nome do executavel que sera gerado, nao troque
TMPOUT=tp1.testresult
BUILD=./build/
SRC=./src/
INCLUDE=./include/
ARGCHCKR=arguments/
PRIMES=primes/


$(EXEC):	$(BUILD)main.o $(BUILD)$(ARGCHCKR)ArgumentsChecker.o $(BUILD)$(PRIMES)ErastothenesSieve.o 
	$(CC) $(CFLAGS) -fopenmp -o $(EXEC) $(BUILD)main.o $(BUILD)*/*.o

$(BUILD)main.o:	$(SRC)main.cpp $(BUILD)$(ARGCHCKR)ArgumentsChecker.o
	$(CC) $(CFLAGS) -I $(INCLUDE)$(ARGCHCKR) -I $(INCLUDE)$(PRIMES) -c $(SRC)main.cpp -fopenmp -o $(BUILD)main.o

$(BUILD)$(ARGCHCKR)ArgumentsChecker.o:	$(SRC)$(ARGCHCKR)ArgumentsChecker.cpp
	$(CC) $(CFLAGS) -I $(INCLUDE)$(ARGCHCKR) -c $(SRC)$(ARGCHCKR)ArgumentsChecker.cpp -o $(BUILD)$(ARGCHCKR)ArgumentsChecker.o

$(BUILD)$(PRIMES)ErastothenesSieve.o:	$(SRC)$(PRIMES)ErastothenesSieve.cpp
	$(CC) $(CFLAGS) -I $(INCLUDE)$(PRIMES) -fopenmp -c $(SRC)$(PRIMES)ErastothenesSieve.cpp -o $(BUILD)$(PRIMES)ErastothenesSieve.o

clean:
	rm -f $(BUILD)*/*.o $(BUILD)*.o

mem_parallel:
	valgrind --leak-check=full --show-leak-kinds=all $(EXEC) t 4

mem_sequential:
	valgrind --leak-check=full --show-leak-kinds=all $(EXEC) t 4
