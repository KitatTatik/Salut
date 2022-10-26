CC=gcc
CFLAGS=-c -g -O0 -Wall -I.
DEP=htable.h

SRC=ht1.c ht2.c
OBJ=$(SRC:.c=.o)
EXE=hth

all: $(EXE)

$(EXE): $(OBJ)
	  $(CC) $^ $(LDLIBS) -o $@

%.o : %.c $(DEP)
	  $(CC) $(CFLAGS) $< -o $@

clean:
	  rm -f $(EXE) $(OBJ)
