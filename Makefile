CCC = clang++
CCFLAGS = -std=c++14 -Wunused-command-line-argument
LLVMFLAGS = `$$LLVM_HOME/bin/llvm-config --cxxflags --ldflags --libs --system-libs`

all: bfcompiler

bfcompiler: token_class.o  bflexer.o bfparser.o bfnode.o IRgenerator.o driver.o
	${CCC} ${CCFLAGS} ${LLVMFLAGS} token_class.o driver.o bflexer.o bfparser.o bfnode.o IRgenerator.o -o bfcompiler

token_class.o: token_class.cc token_class.h bfspec.h
	${CCC}  ${CCFLAGS} ${LLVMFLAGS} -c token_class.cc

bflexer.o: bflexer.cc bflexer.h bfspec.h token_class.h
	${CCC} ${CCFLAGS} ${LLVMFLAGS} -c bflexer.cc

bfparser.o: bfparser.cc bfparser.h token_class.h bfspec.h
	${CCC} ${CCFLAGS} ${LLVMFLAGS} -c bfparser.cc

bfnode.o: bfnode.h bfnode.cc bfspec.h
	${CCC} ${CCFLAGS} ${LLVMFLAGS} -c bfnode.cc

IRgenerator.o : IRgenerator.h IRgenerator.cc bfnode.h
	${CCC} ${CCFLAGS} ${LLVMFLAGS} -c IRgenerator.cc -frtti

driver.o: driver.cc token_class.h bflexer.h bfparser.h bfnode.h token_class.cc bflexer.cc bfparser.cc bfnode.cc IRgenerator.h IRgenerator.cc
	${CCC} ${CCFLAGS} ${LLVMFLAGS} -c driver.cc

.PHONY:	clean
clean:
	rm -f *.o *.gch bfcompiler a.out