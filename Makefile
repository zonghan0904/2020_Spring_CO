CC = g++
EXE = FMIndex.o
SRC = FMIndex.cpp

all: $(SRC)
	$(CC) $^ -o $(EXE)

generate: generator.py
	python $^ | tee COsmall.txt

.PHONY: clean
clean:
	rm $(EXE)
