#ifndef SERVER_H_
#define SERVER_H_
#include "team5_common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <iostream>
#include <vector>
using namespace std;

struct client_record//This struct keeps record about a single client 
{
	int socket_descriptor;
	char username[16];
	int status;
};

void send_forward_message(int sockfd, char* username, char* message);//This function sends a forward message to client
void send_ack_message(int sockfd, unsigned short int client_count, char* username);//This function sends an ack message to client
void send_nak_message(int sockfd, char* reason);//This function sends a nack message to client
void send_online_message(int sockfd, char* username);//This function sends an online message to client
void send_offline_message(int sockfd, char* username);//This function sends an offline message to client
void send_idle_message(int sockfd, char* username);//This function sends an idle message to client
void handle_new_connection(int listenfd, sockaddr_in client_addr, fd_set &allset, int &maxfd);//This function handles new connections
void got_data(unsigned short int &client_count, int descriptor, fd_set &allset, int &maxfd, int max_clients);//This function handles already connected clients

#endif
