# bfcompiler
brainfuck-compiler written in c/c++

## Language Specifications
  * Addition and Subtraction are the only atomic operations present, there are no abstractions like multiplication and division 
  * Language character-set : +-<>[].,
  * The tape has 30000 cells
  
## Compilation Instructions
  * set-up llvm on the host system : version used for this project-10.0.0
    * download clang-llvm for appropriate architecture from https://releases.llvm.org/download.html#10.0.0
    * extract the archive
    * add env-variable LLVM_HOME to the location where its extracted.
  * add LLVM_HOME/bin to $PATH if llvm-config and clang++ are not installed already.
 
##### This will generate brainfuck-compiler binary bfcompiler
  ```
  lol@lol$ make clean
  lol@lol$ make
  ```
##### Compile the sample-bf program to host-architecture object-file `output.o`
  * this will print llvm-IR code for the mainFunction of llvm-IR representation and
  generate object file.
  ```
  lol@lol$ ./bfcompiler hellow.bf
  ```
  
##### Link the Object file to produce executable a.out
```
lol@lol$ clang output.o
```

##### Run executable to see output of brainfuck code
```
lol@lol$ ./a.out 
Hello World!
lol@lol$
```
hence compiling the brainfuck code to the host-architecture binary.