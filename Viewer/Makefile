CC=g++
FLAGS=-std=c++14
LIBS=-pthread -lboost_system -lglut -lGL -lGLU -lX11
COMM_DIR=../Communication

all: client_viewer

client_viewer:
	$(CC) viewer.cpp $(COMM_DIR)/ViewerClient.cpp $(COMM_DIR)/Utility.cpp \
		 $(COMM_DIR)/Client.cpp -o Viewer $(FLAGS) $(LIBS)

clean:
	rm -f *.o Viewer

