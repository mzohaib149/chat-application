#ifndef CLIENT_H_
#define CLIENT_H_
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
using namespace std;

void send_join_message(int sockfd, char* username);//This function sends a join message to server
void send_send_message(int sockfd, char* message);//This function sends a send message to server
void send_idle_message(int sockfd);//This function sends an idle message to server
void got_data(int descriptor, bool &nak_received);//This function checks whether client has received a FWD, ACK, NAK, ONLINE, OFFLINE or IDLE message and performs accordingly

#endif
