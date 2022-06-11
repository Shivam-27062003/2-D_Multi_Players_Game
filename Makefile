CC = g++
CLIENT_OBJ = main.cpp
CLIENT_FLAGS = -w -pthread -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
CLIENT_OUT = main.out

SERVER_OBJ = server.cpp
SERVER_FLAGS = -w -pthread
SERVER_OUT = server.out

all:
	$(CC) $(CLIENT_OBJ) $(CLIENT_FLAGS) -o $(CLIENT_OUT)
	$(CC) $(SERVER_OBJ) $(SERVER_FLAGS) -o $(SERVER_OUT)