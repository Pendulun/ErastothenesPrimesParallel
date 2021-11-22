CC=g++ # compilador, troque para gcc se preferir utilizar C
CFLAGS=-Wall -g #-Wextra # compiler flags, troque o que quiser, exceto bibliotecas externas
EXEC=./primes # nome do executavel que sera gerado, nao troque
TMPOUT=tp1.testresult
BUILD=./build/
SRC=./src/
INCLUDE=./include/
ARGCHCKR=arguments/

# $(EXEC):	$(BUILD)main.o  $(BUILD)$(COMPR)CompressorLZ78.o $(BUILD)$(COMPR)Compressor.o
# 	$(CC) $(CFLAGS) -o $(EXEC) $(BUILD)main.o $(BUILD)*/*.o 

# $(BUILD)main.o:	$(SRC)main.cpp $(BUILD)$(VALID)Validador.o  $(BUILD)$(COMPR)Compressor.o $(BUILD)$(COMPR)CompressorLZ78.o
# 	$(CC) $(CFLAGS) -I $(INCLUDE)$(VALID) -I $(INCLUDE)$(COMPR) -c $(SRC)main.cpp -o $(BUILD)main.o


$(EXEC):	$(BUILD)main.o $(BUILD)$(ARGCHCKR)ArgumentsChecker.o
	$(CC) $(CFLAGS) -o $(EXEC) $(BUILD)main.o $(BUILD)*/*.o

$(BUILD)main.o:	$(SRC)main.cpp $(BUILD)$(ARGCHCKR)ArgumentsChecker.o
	$(CC) $(CFLAGS) -I $(INCLUDE)$(ARGCHCKR) -c $(SRC)main.cpp -o $(BUILD)main.o

$(BUILD)$(ARGCHCKR)ArgumentsChecker.o:	$(SRC)$(ARGCHCKR)ArgumentsChecker.cpp
	$(CC) $(CFLAGS) -I $(INCLUDE)$(ARGCHCKR) -c $(SRC)$(ARGCHCKR)ArgumentsChecker.cpp -o $(BUILD)$(ARGCHCKR)ArgumentsChecker.o

# $(BUILD)$(VALID)Validador.o: $(SRC)$(VALID)Validador.cpp
# 	$(CC) $(CFLAGS) -I $(INCLUDE)$(VALID) -c $(SRC)$(VALID)Validador.cpp -o $(BUILD)$(VALID)Validador.o

# $(BUILD)$(COMPR)CompressorLZ78.o: $(SRC)$(COMPR)CompressorLZ78.cpp $(INCLUDE)$(COMPR)CompressorLZ78.hpp $(BUILD)$(COMPR)Compressor.o $(BUILD)$(COMPR)TrieEncoder.o $(BUILD)$(COMPR)Trie.o $(BUILD)$(COMPR)Retorno.o
# 	$(CC) $(CFLAGS) -I $(INCLUDE)$(COMPR) -c $(SRC)$(COMPR)CompressorLZ78.cpp -o $(BUILD)$(COMPR)CompressorLZ78.o

# $(BUILD)$(COMPR)Compressor.o: $(SRC)$(COMPR)Compressor.cpp $(INCLUDE)$(COMPR)Compressor.hpp
# 	$(CC) $(CFLAGS) -I $(INCLUDE)$(COMPR) -c $(SRC)$(COMPR)Compressor.cpp -o $(BUILD)$(COMPR)Compressor.o

# $(BUILD)$(COMPR)TrieEncoder.o: $(SRC)$(COMPR)TrieEncoder.cpp $(INCLUDE)$(COMPR)TrieEncoder.hpp $(BUILD)$(COMPR)Trie.o $(BUILD)$(COMPR)Node.o $(BUILD)$(COMPR)Retorno.o
# 	$(CC) $(CFLAGS) -I $(INCLUDE)$(COMPR) -c $(SRC)$(COMPR)TrieEncoder.cpp -o $(BUILD)$(COMPR)TrieEncoder.o

# $(BUILD)$(COMPR)Trie.o: $(SRC)$(COMPR)Trie.cpp $(INCLUDE)$(COMPR)Trie.hpp $(BUILD)$(COMPR)Retorno.o
# 	$(CC) $(CFLAGS) -I $(INCLUDE)$(COMPR) -c $(SRC)$(COMPR)Trie.cpp -o $(BUILD)$(COMPR)Trie.o

# $(BUILD)$(COMPR)Node.o: $(SRC)$(COMPR)Node.cpp $(INCLUDE)$(COMPR)Node.hpp
# 	$(CC) $(CFLAGS) -I $(INCLUDE)$(COMPR) -c $(SRC)$(COMPR)Node.cpp -o $(BUILD)$(COMPR)Node.o

# $(BUILD)$(COMPR)Retorno.o: $(SRC)$(COMPR)Retorno.cpp $(INCLUDE)$(COMPR)Retorno.hpp $(BUILD)$(COMPR)Node.o
# 	$(CC) $(CFLAGS) -I $(INCLUDE)$(COMPR) -c $(SRC)$(COMPR)Retorno.cpp -o $(BUILD)$(COMPR)Retorno.o

# test:	$(EXEC)
# 	@bash run_tests.sh $(EXEC) $(TMPOUT)

clean:
	rm -f $(BUILD)*/*.o $(BUILD)*.o

mem_parallel:
	valgrind --leak-check=full --show-leak-kinds=all $(EXEC) t 4

mem_sequential:
	valgrind --leak-check=full --show-leak-kinds=all $(EXEC) t 4
