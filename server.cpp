// Server side C/C++ program to demonstrate Socket
// programming
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include<vector>
#include<pthread.h>
#include<iostream>
#include<set>
using namespace std;
#define PORT 8080

pthread_t listeningThread;
set<int> sockets;
set<int> Terminated;
int i = 0;
int server_fd, valread;
sockaddr_in address;
int opt = 1;
int addrlen = sizeof(address);
bool terminate = false;


void* HandleClient(void *args){
    cerr << "Handling client" << endl;
	char buffer[1024];
	int *socketPointer =  (int*) args;
	int newSocket = *socketPointer;
	while (true){
		valread = read(newSocket, buffer, 1024);
		string temp(buffer);
		if (temp.length() < 6) continue;
        //cerr << "Received: " << temp << " from " << newSocket << endl;
		cerr<<"Running...\n";
		string init(temp.begin(), temp.begin() + 3);
		if (init != "###") continue;
		string s(temp.begin() + 3, temp.end());
        s = to_string(newSocket) + " " + s;
		s = "###" + s;
		for (int x: sockets){
			if (x == newSocket || Terminated.find(x) != Terminated.end()) continue;
			send(x, s.c_str(), strlen(s.c_str()), 0);
		}
		if (temp == "###Terminate###") {
			Terminated.insert(newSocket);
			return NULL;
		}
        for (int i = 0; i < 1024; i++) buffer[i] = 0;
	}
	return NULL;
}

int main(int argc, char const* argv[])
{
	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0))
		== 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// Forcefully attaching socket to the port 8080
	if (setsockopt(server_fd, SOL_SOCKET,
				SO_REUSEADDR | SO_REUSEPORT, &opt,
				sizeof(opt))) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	// Forcefully attaching socket to the port 8080
	if (bind(server_fd, (struct sockaddr*)&address,
			sizeof(address))
		< 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	
	
	while (true){
		if (listen(server_fd, 3) < 0) {
			perror("listen");
			exit(EXIT_FAILURE);
		}
		int newSocket;
		if ((newSocket
			= accept(server_fd, (struct sockaddr*)&address,
					(socklen_t*)&addrlen))
			< 0) {
			perror("accept");
			exit(EXIT_FAILURE);
		}
		pthread_create(&listeningThread, NULL, HandleClient, (void*)(&newSocket));
		sockets.insert(newSocket);
	}
	return 0;
}