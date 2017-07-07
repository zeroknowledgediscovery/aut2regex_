# A C++ implementation to find a simple regular expression for an automata using Brzozowski algebraic method.


    make config.o
    make regx.o
    make

This makes a static  library `libregx.a` in `\lib`. We also build `libconfig.a` to coveniently read automata specification files.
Some example files is given in `\test`.

The binary is placed in `\bin`

    \bin\script_ \test\aut1.dat

## Format of input

   #CONNX  % this line needs to be here
   0 1
   1 0

This is simply the state connectivity matrix. State numbering begins from zero.
If state is not connected fully, use -1 (experimental)

The weird header CONNX is there because the
reader is from another related project.

