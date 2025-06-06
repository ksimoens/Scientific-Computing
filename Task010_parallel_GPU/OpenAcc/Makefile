COMP=gcc

# if the compiler is the normal GNU C compiler
ifeq ($(COMP),gcc)
# set the GNU compiler as default
CC=gcc
# set the compiler flags:
# -g for debugging
# -O3 for optimisation
# -fopenacc for the OpenAcc implementation
CCFLAGS=-g -O3 -fopenacc
RUN=
endif

# the name of the executable(s)
EXE=daxpy

# for all the executables in the directory
all: $(EXE)

# do the compilation:
# main: main.c
#	gcc -g -O3 -fopenacc main.c -o main
$(EXE): $(EXE).c
	$(CC) $(CCFLAGS) $^ -o $@ 

# clean the directory = remove executables and .o files
clean:
	rm -f $(EXE) *.o *~