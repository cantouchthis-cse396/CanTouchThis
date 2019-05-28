CC=g++
FLAGS=-std=c++14
LIBS=-pthread -lboost_system

all: utility server client_scanner client_conversion client_viewer client_controller

utility:
	$(CC) -c Utility.cpp $(FLAGS)

server:
	$(CC) -c CommServer.cpp ServerTest.cpp Utility.cpp $(FLAGS)
	$(CC) CommServer.o ServerTest.o Utility.o -o Server $(LIBS)

client:
	$(CC) -c Client.cpp $(FLAGS)

client_scanner:
	$(CC) -c ScannerClient.cpp $(FLAGS)

client_conversion:
	$(CC) -c ConversionClient.cpp ConversionTest.cpp $(FLAGS)

client_viewer:
	$(CC) -c ViewerClient.cpp ViewerTest.cpp $(FLAGS)

client_controller:
	$(CC) -c ControllerClient.cpp ControllerTest.cpp $(FLAGS)

clean:
	rm -f *.o Server

