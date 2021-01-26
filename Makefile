all: bfcompiler

bfcompiler: token_class.o  bflexer.o bfparser.o bfnode.o driver.o
	clang++ -std=c++11 -o bfcompiler token_class.o driver.o bflexer.o bfparser.o bfnode.o

token_class.o: token_class.cc token_class.h bfspec.h
	clang++  -std=c++11 -c token_class.cc

bflexer.o: bflexer.cc bflexer.h bfspec.h token_class.h
	clang++ -std=c++11 -c bflexer.cc

bfparser.o: bfparser.cc bfparser.h token_class.h bfspec.h
	clang++ -std=c++11 -c bfparser.cc

bfnode.o: bfnode.h bfnode.cc bfspec.h
	clang++ -std=c++11 -c bfnode.cc

driver.o: driver.cc token_class.h bflexer.h bfparser.h bfnode.h token_class.cc bflexer.cc bfparser.cc bfnode.cc
	clang++ -std=c++11 -c driver.cc

.PHONY:	clean
clean:
	rm -f *.o *.gch bfcompiler