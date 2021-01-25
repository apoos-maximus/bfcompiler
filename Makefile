all: bfcompiler

bfcompiler: token_class.o driver.o bflexer.o bfparser.o
	clang++ -o bfcompiler token_class.o driver.o bflexer.o bfparser.o

token_class.o: token_class.cc token_class.h bfspec.h
	clang++ -c token_class.cc

bflexer.o: bflexer.cc bflexer.h bfspec.h token_class.h
	clang++ -c bflexer.cc

bfparser.o: bfparser.cc bfparser.h token_class.h bfspec.h
	clang++ -c bfparser.cc

driver.o: driver.cc token_class.h bflexer.h bfparser.h
	clang++ -c driver.cc

.PHONY:	clean
clean:
	rm -f *.o *.gch bfcompiler