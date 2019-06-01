CC=g++
FLAGS=-std=c++14
LIBS=-pthread -lboost_system -lwiringPi

all:
	$(CC) ../Communication/ScannerClient.o ../Communication/Client.o \
		../Communication/Utility.o scanner.cpp -o scanner $(LIBS)
clean:
	rm server

