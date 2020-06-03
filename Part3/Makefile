CC = g++
EXE = Search
SRC = FMIndex.cpp main.cpp
OBJ = FMIndex.o main.o
INCDIR = ./

all: $(OBJ)
	$(CC) $^ -o $(EXE)

%.o: %.c
	$(CC) -I$(INCDIR) -c $^

generate: generator.py
	python $^ | tee COsmall.txt

.PHONY: clean
clean:
	rm $(EXE) $(OBJ)
