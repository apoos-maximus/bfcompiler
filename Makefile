all: bfcompiler

bfcompiler: token_class.o  bflexer.o bfparser.o bfnode.o IRgenerator.o driver.o
	clang++ -std=c++14 -o bfcompiler -mlinker-version=609.8 token_class.o driver.o bflexer.o bfparser.o bfnode.o IRgenerator.o

token_class.o: token_class.cc token_class.h bfspec.h
	clang++  -std=c++14 -c token_class.cc

bflexer.o: bflexer.cc bflexer.h bfspec.h token_class.h
	clang++ -std=c++14 -c bflexer.cc

bfparser.o: bfparser.cc bfparser.h token_class.h bfspec.h
	clang++ -std=c++14 -c bfparser.cc

bfnode.o: bfnode.h bfnode.cc bfspec.h
	clang++ -std=c++14 -c bfnode.cc

IRgenerator.o : IRgenerator.h IRgenerator.cc bfnode.h
	clang++ -std=c++14 -c `$$LLVM_HOME/bin/llvm-config --cxxflags  --ldflags --libs --system-libs` IRgenerator.cc -frtti

driver.o: driver.cc token_class.h bflexer.h bfparser.h bfnode.h token_class.cc bflexer.cc bfparser.cc bfnode.cc IRgenerator.h IRgenerator.cc
	clang++ -std=c++14 -c ` $$LLVM_HOME/bin/llvm-config --cxxflags  --ldflags --libs --system-libs` driver.cc

.PHONY:	clean
clean:
	rm -f *.o *.gch bfcompiler