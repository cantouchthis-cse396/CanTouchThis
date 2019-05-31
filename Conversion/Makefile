CC=g++
FLAGS=-std=c++14
LIBS=-pthread -lboost_system								# Dynamic linking
COMM_DIR=../Communication

all: client_conversion

client_conversion:
	$(CC) conversion.cpp $(COMM_DIR)/ConversionClient.cpp $(COMM_DIR)/Utility.cpp \
		 $(COMM_DIR)/Client.cpp -o Conversion $(FLAGS) $(LIBS)

clean:
	rm -f *.o Conversion


