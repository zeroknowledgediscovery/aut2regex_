CC = g++ -std=c++11 

CFLAGS = -O3 -Wall -Wextra -Wunused  -lgsl -lgslcblas -lm  -lgomp 
#-lboost_thread
CFLAGST =  -O2 -Wall -Wextra -Wunused  -lgsl -lgslcblas -lm  -lgomp -lboost_thread
CFLAGSNOP = -g -Wall -Wextra -Wunused  -lgsl -lgslcblas -lm  -lgomp -lboost_thread

DEPS = 

INCLUDES = -I /usr/include/boost/graph    /usr/include/boost/random  
# /usr/include/boost/thread

OBJ =  script_ 

all:	 script_  clear mvbin

# compile libraries --------------------------------

semantic.o: semantic.cc $(DEPS)
	$(CC) -c -o $@ $< -fopenmp $(CFLAGS) $(INCLUDES); ar rcs libsemcrct.a semantic.o; rm semantic.o; mv libsemcrct.a ./lib

config.o: config.cc $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS); ar rcs libconfigfile.a config.o; rm config.o; mv libconfigfile.a ./lib

regx.o: regx.cc $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS); ar rcs libregx.a regx.o; rm regx.o; mv libregx.a ./lib


# compile binaries --------------------------------


script_.o: script_.cc
	$(CC) -c -o $@ $< -fopenmp $(CFLAGS)

script_: script_.o
	$(CC)  $(CFLAGS) -o $@ $^ -lconfigfile -lregx -L./lib 
# -lboost_program_options


# utility commands --------------------------------

clear:	
	rm -rf *.o
clean:	
	rm -rf *.o *~ $(OBJ) *.tgz

mvbin:
	rm -rf ./bin >&/dev/null; mkdir bin; mv $(OBJ) ./bin
src:
	tar -czvf src.tgz *.cc *.h Makefile Doxyfile
ref:
	doxygen Doxyfile; cd doc/latex; make; make

# EOF --------------------------------
