all: bfcompiler

bfcompiler: token_class.o driver.o
	clang++ -o bfcompiler token_class.o driver.o

token_class.o: token_class.cc token_class.h bfspec.h
	clang++ -c token_class.cc

driver.o: driver.cc token_class.h
	clang++ -c driver.cc


.PHONY:	clean
clean:
	rm -f *.o *.gch bfcompiler