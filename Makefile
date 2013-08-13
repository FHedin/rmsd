#################################################################
########################   MakeVars   ###########################
#################################################################

CC=gcc

CC_OPT=-I"./include" -m64 -std=c99 -Wall -O2 #-g #-O3 -funroll-loops -march=native

LD_LIB=#-L"/usr/lib64/atlas-sse3"

LD_OPT=-lm -llapack -lblas #-lf77blas -lcblas -latlas

MKDIR=mkdir -p ./obj

CIBLE=rmsd

SRC=$(wildcard ./src/*.c)

OBJ=$(patsubst ./src/%.c,./obj/%.o,$(SRC))

#################################################################
########################   Makefile   ###########################
#################################################################

all:$(CIBLE)
	@echo "Compilation Success"

$(CIBLE):Makefile

./obj/%.o:./src/%.c
	@$(MKDIR)
	$(CC) $(CC_OPT) -c $< -o $@

$(CIBLE):$(OBJ)
	$(CC) $(CC_OPT) $(LD_LIB) $(OBJ) -o $@ $(LD_OPT)

clean:
	rm -f $(CIBLE) ./obj/*.o

rs:
	rsync -avz ./src ~/Dropbox/tools/rmsd
	rsync -avz ./include ~/Dropbox/tools/rmsd
	rsync -avz ./Makefile ~/Dropbox/tools/rmsd
