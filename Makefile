#Makefile
CC=g++
BIN=./bin/
SRC=./src/
STD=-std=c++11

all: bin main

bin:
	mkdir -p bin
main:
	$(CC) $(STD) $(SRC)main.cpp $(SRC)lines.cpp $(SRC)recovery.cpp -o $(BIN)main -lmysqlclient `mysql_config --cflags --libs` -Wno-write-strings

clean:
	rm -rf $(BIN)
