#include "team5_common.h"
#include "team5_server.h"

vector<client_record> client_list;

void send_forward_message(int sockfd, char* username, char* message)
{
	struct SBCP_FWD_Message  sfm;
	sfm.hdr.Vrsn_and_Type = htons(VERSION3TYPE3);
	sfm.hdr.Length = htons(strlen(username)+strlen(message)+12+2);
	sfm.attr_hdr1.Type = htons(2);//for username
	sfm.attr_hdr1.Length = htons(strlen(username)+1);
	strcpy(sfm.Username,username);
	sfm.attr_hdr2.Type = htons(4);//for message
	sfm.attr_hdr2.Length = htons(strlen(message)+1);
	strcpy(sfm.Message,message);
	
	int buffer_size = 0;	
	buffer_size = strlen(username)  + strlen(message)  + 12 + 2;
	char buffer[buffer_size];
	memset(buffer, 0, buffer_size);
	memcpy(&buffer[0], &sfm.hdr.Vrsn_and_Type, 2);
	memcpy(&buffer[2], &sfm.hdr.Length, 2);
	memcpy(&buffer[4], &sfm.attr_hdr1.Type, 2);
	memcpy(&buffer[6], &sfm.attr_hdr1.Length, 2);
	memcpy(&buffer[8], sfm.Username, strlen(sfm.Username));
	int i;
	i = strlen(sfm.Username) + 8 + 1;
	memcpy(&buffer[i], &sfm.attr_hdr2.Type, 2);
	memcpy(&buffer[i+2], &sfm.attr_hdr2.Length, 2);
	memcpy(&buffer[i+4], sfm.Message, strlen(sfm.Message));
	
	int bytes_sent = send(sockfd, buffer, buffer_size, 0);
	cout<<"sent "<<bytes_sent<<" bytes of forward message"<<endl; 	
	
}

void send_ack_message(int sockfd, unsigned short int client_count, char* username)
{
	struct SBCP_ACK_Message  sam;
	sam.hdr.Vrsn_and_Type = htons(VERSION3TYPE7);
	sam.hdr.Length = htons(strlen(username)+15);
	sam.attr_hdr1.Type = htons(3);//for client count
	sam.attr_hdr1.Length = htons(2);
	sam.Client_Count = client_count;
	sam.attr_hdr2.Type = htons(2);//for list of usernames
	sam.attr_hdr2.Length = htons(strlen(username)+1);
	strcpy(sam.Username,username);
	int buffer_size = 0;	
	buffer_size = strlen(username) + 1 + 14;
	char buffer[buffer_size];
	memset(buffer, 0, buffer_size);
	memcpy(&buffer[0], &sam.hdr.Vrsn_and_Type, 2);
	memcpy(&buffer[2], &sam.hdr.Length, 2);
	memcpy(&buffer[4], &sam.attr_hdr1.Type, 2);
	memcpy(&buffer[6], &sam.attr_hdr1.Length, 2);
	memcpy(&buffer[8], &sam.Client_Count, 2);
	memcpy(&buffer[10], &sam.attr_hdr2.Type, 2);
	memcpy(&buffer[12], &sam.attr_hdr2.Length, 2);
	memcpy(&buffer[14], sam.Username, strlen(sam.Username));
	
	int bytes_sent = send(sockfd, buffer, buffer_size, 0);
	cout<<"sent "<<bytes_sent<<" bytes of ack message"<<endl; 	
	
}

void send_nak_message(int sockfd, char* reason)
{
	struct SBCP_NAK_Message  snm;
	snm.hdr.Vrsn_and_Type = htons(VERSION3TYPE5);
	snm.hdr.Length = htons(strlen(reason)+9);
	snm.attr_hdr.Type = htons(1);//for reason
	snm.attr_hdr.Length = htons(strlen(reason)+1);
	strcpy(snm.Reason,reason);
	int buffer_size = 0;
	buffer_size = strlen(reason)+ 8 + 1;
	char buffer[buffer_size];
	memset(buffer, 0, buffer_size);
	memcpy(&buffer[0], &snm.hdr.Vrsn_and_Type, 2);
	memcpy(&buffer[2], &snm.hdr.Length, 2);
	memcpy(&buffer[4], &snm.attr_hdr.Type, 2);
	memcpy(&buffer[6], &snm.attr_hdr.Length, 2);
	memcpy(&buffer[8], snm.Reason, strlen(snm.Reason));
	int bytes_sent = send(sockfd, buffer, buffer_size, 0);
	cout<<"sent "<<bytes_sent<<" bytes of nak message"<<endl; 	
	
}

void send_online_message(int sockfd, char* username)
{
	struct SBCP_ONLINE_Message  sonm;
	sonm.hdr.Vrsn_and_Type = htons(VERSION3TYPE8);
	sonm.hdr.Length = htons(strlen(username)+9);
	sonm.attr_hdr.Type = htons(2);//for username
	sonm.attr_hdr.Length = htons(strlen(username)+1);
	strcpy(sonm.Username,username);
	int buffer_size = 0;
	buffer_size = strlen(username)+ 8 + 1;
	char buffer[buffer_size];
	
	memset(buffer, 0, buffer_size);
	memcpy(&buffer[0], &sonm.hdr.Vrsn_and_Type, 2);
	memcpy(&buffer[2], &sonm.hdr.Length, 2);
	memcpy(&buffer[4], &sonm.attr_hdr.Type, 2);
	memcpy(&buffer[6], &sonm.attr_hdr.Length, 2);
	memcpy(&buffer[8], sonm.Username, strlen(sonm.Username));
	int bytes_sent = send(sockfd, buffer, buffer_size, 0);
	cout<<"sent "<<bytes_sent<<" bytes of online message"<<endl; 	
	
}

void send_offline_message(int sockfd, char* username)
{
	struct SBCP_OFFLINE_Message  sofm;
	sofm.hdr.Vrsn_and_Type = htons(VERSION3TYPE6);
	sofm.hdr.Length = htons(strlen(username)+9);
	sofm.attr_hdr.Type = htons(2);//for username
	sofm.attr_hdr.Length = htons(strlen(username)+1);
	strcpy(sofm.Username,username);
	int buffer_size = 0;
	buffer_size = strlen(username)+ 8 + 1;
	char buffer[buffer_size];
	
	memset(buffer, 0, buffer_size);
	memcpy(&buffer[0], &sofm.hdr.Vrsn_and_Type, 2);
	memcpy(&buffer[2], &sofm.hdr.Length, 2);
	memcpy(&buffer[4], &sofm.attr_hdr.Type, 2);
	memcpy(&buffer[6], &sofm.attr_hdr.Length, 2);
	memcpy(&buffer[8], sofm.Username, strlen(sofm.Username));
	int bytes_sent = send(sockfd, buffer, buffer_size, 0);
	cout<<"sent "<<bytes_sent<<" bytes of offline message"<<endl; 	
	
}

void send_idle_message(int sockfd, char* username)
{
	struct SBCP_IDLE_Message_Server  sim;
	sim.hdr.Vrsn_and_Type = htons(VERSION3TYPE9);
	sim.hdr.Length = htons(strlen(username)+9);
	sim.attr_hdr.Type = htons(2);//for username
	sim.attr_hdr.Length = htons(strlen(username)+1);
	strcpy(sim.Username,username);
	int buffer_size = 0;
	buffer_size = strlen(username)+ 8 + 1;
	char buffer[buffer_size];
	
	memset(buffer, 0, buffer_size);
	memcpy(&buffer[0], &sim.hdr.Vrsn_and_Type, 2);
	memcpy(&buffer[2], &sim.hdr.Length, 2);
	memcpy(&buffer[4], &sim.attr_hdr.Type, 2);
	memcpy(&buffer[6], &sim.attr_hdr.Length, 2);
	memcpy(&buffer[8], sim.Username, strlen(sim.Username));
	int bytes_sent = send(sockfd, buffer, buffer_size, 0);
	cout<<"sent "<<bytes_sent<<" bytes of idle message"<<endl; 		
	
}

void handle_new_connection(int listenfd, sockaddr_in client_addr, fd_set &allset, int &maxfd)
{
	cout<<"got a new connection"<<endl;
	socklen_t addr_len = sizeof(client_addr);
	int newsock;
	if ((newsock = accept(listenfd, (struct sockaddr*)& client_addr, &addr_len))<0)
		perror("accept error"); 

	FD_SET(newsock, &allset);
	
	if (newsock>maxfd)
	{
		maxfd = newsock;
	}
}

void got_data(unsigned short int &client_count, int descriptor, fd_set &allset, int &maxfd, int max_clients)
{
	char header_buffer[4];
	int nbytes = recv(descriptor, header_buffer, 4, 0); 
	cout<<"received "<<nbytes<<" bytes of header"<<endl; 	
	if (nbytes == 0)// connection is closed by some client
	{
		//create offline message and send to other clients
		int count1=0;
		bool flag = false;
		char temp[16];
		for (int count=0; count<client_list.size(); count++)
		{
			if (client_list[count].socket_descriptor == descriptor)
			{
				flag = true;
				count1 = count;
				strcpy(temp,client_list[count].username);
				break;
			} 
		}
		if (flag)
		{
			client_list.erase(client_list.begin()+count1);
		}
		for (int count2=0; count2<client_list.size(); count2++)
		{
			send_offline_message(client_list[count2].socket_descriptor,temp);	
		}
		close(descriptor);
		FD_CLR(descriptor,&allset);
		client_count--;
	}
	else
	{
	struct SBCP_Header h;
	memcpy(&h.Vrsn_and_Type, &header_buffer[0], 2);
	memcpy(&h.Length, &header_buffer[2], 2);
	unsigned short int temp_vrsn_and_type = ntohs(h.Vrsn_and_Type);
	h.Vrsn_and_Type = temp_vrsn_and_type;
	unsigned short int temp_length = ntohs(h.Length);
	h.Length = temp_length;
		
	if (((h.Vrsn_and_Type) & MASK) == 2)//received a join
	{
		int i = h.Length - 4;
		char payload_buffer[i];
		struct SBCP_JOIN_Message sjm;
		sjm.hdr.Vrsn_and_Type = h.Vrsn_and_Type;
		sjm.hdr.Length = h.Length;
		int nbytes1 = recv(descriptor, payload_buffer, i, 0); 
		cout<<"received "<<nbytes1<<" bytes of join payload"<<endl;
		memcpy(&sjm.attr_hdr.Type, &payload_buffer[0], 2);
		memcpy(&sjm.attr_hdr.Length, &payload_buffer[2], 2);
		memcpy(&sjm.Username, &payload_buffer[4],ntohs(sjm.attr_hdr.Length));
		sjm.attr_hdr.Type = ntohs(sjm.attr_hdr.Type);
		sjm.attr_hdr.Length =  ntohs(sjm.attr_hdr.Length);
		//finished receiving JOIN and filled the struct
		//if the client_list is empty
		//put create a new record and put in the list
		if (client_list.empty())
		{
			cout<<"chat room empty"<<endl;
			cout<<"accepted the client"<<endl;
			struct client_record cr;
			cr.socket_descriptor = descriptor;
			strcpy(cr.username,sjm.Username);
			cr.status = ONLINE;			
			client_list.push_back(cr);
			client_count++;
			char clients[8];
			strcpy(clients, "<empty>");
						
			send_ack_message(descriptor, client_count, clients);
			
					
		}
		else
		{
			if (client_count == max_clients)//max capacity reached
			{
				cout<<"chat room full"<<endl;
				char reason_string[32];
				strcpy(reason_string,"chatroom is full");
				send_nak_message(descriptor,reason_string);
				
				close(descriptor);
				FD_CLR(descriptor,&allset);
			}
			else
			{	
			 	bool flag=false;
				for (int count1=0; count1<client_list.size();count1++)
				{
					if (!(strcmp(client_list[count1].username,sjm.Username)))
					{
						flag=true;
						break;
					}
				}
				if (flag)//if the username already exists in the list, send NAK, close the connection and remove descriptor from fd_set
				{
					cout<<"username already exists"<<endl;
					char reason_string[32];
					strcpy(reason_string, "user already exists");
					send_nak_message(descriptor,reason_string);
					
					close(descriptor);
					FD_CLR(descriptor,&allset);
				}
				else//if the username does not exist, create a new user record and put it in the list, increase client count, send ACK to sender of message, send ONLINE to all other client
				{
					cout<<"accepted the user"<<endl;
					struct client_record cr;
					cr.socket_descriptor = descriptor;
					strcpy(cr.username,sjm.Username);
					cr.status = ONLINE;
					client_list.push_back(cr);
					client_count++;
					char clients[800];
					strcpy(clients, " ");
					for (int count2=0; count2<client_list.size(); count2++)
					{
						if (strcmp(client_list[count2].username,sjm.Username))
						{
							strcat(clients, client_list[count2].username);
							strcat(clients, " ");
						}
					}
					send_ack_message(descriptor, client_count, clients);
					for (int count3=0; count3<client_list.size(); count3++)
					{
						if (strcmp(client_list[count3].username,sjm.Username))
						{
							send_online_message(client_list[count3].socket_descriptor, sjm.Username);
							
						}	
					}
				}
			}	
		}
	}

	if (((h.Vrsn_and_Type) & MASK) == 4)//received a send
	{
		int i = h.Length - 4;
		char payload_buffer[i];
		struct SBCP_SEND_Message ssm;
		ssm.hdr.Vrsn_and_Type = h.Vrsn_and_Type;
		ssm.hdr.Length = h.Length;
		int nbytes1 = recv(descriptor, payload_buffer, i, 0);
		cout<<"received "<<nbytes1<<" bytes of send payload"<<endl;
		 
		memcpy(&ssm.attr_hdr.Type, &payload_buffer[0], 2);
		memcpy(&ssm.attr_hdr.Length, &payload_buffer[2], 2);
		memcpy(&ssm.Message, &payload_buffer[4],ntohs(ssm.attr_hdr.Length));
		ssm.attr_hdr.Type = ntohs(ssm.attr_hdr.Type);
		ssm.attr_hdr.Length =  ntohs(ssm.attr_hdr.Length);
		//finished receiving SEND and filled the struct
		//create FWD message and send to all the other clients
		char temp_username[16];
		for (int count1=0; count1<client_list.size(); count1++)
		{
			if (client_list[count1].socket_descriptor == descriptor)
			{
				strcpy(temp_username, client_list[count1].username);
				break;
			} 
		}
		for (int count2=0; count2<client_list.size(); count2++)
		{
			if (strcmp(client_list[count2].username,temp_username))
			{
				send_forward_message(client_list[count2].socket_descriptor, temp_username, ssm.Message);
				
			}
		}
	}

	if (((h.Vrsn_and_Type) & MASK) == 9)//received an idle
	{
				
		//create idle message and send to all others
		char temp_username[16];
		for (int count1=0; count1<client_list.size(); count1++)
		{
			if (client_list[count1].socket_descriptor == descriptor)
			{
				strcpy(temp_username, client_list[count1].username);
				client_list[count1].status = IDLE;
				break;
			} 
		}
		for (int count2=0; count2<client_list.size(); count2++)
		{
			if (strcmp(client_list[count2].username,temp_username))
			{
				send_idle_message(client_list[count2].socket_descriptor, temp_username);
				
			}
		}	
	}
	}
}



int main(int argc,char** argv)
{
	unsigned short int client_count = 0;
	if (argc!=4)
	{
		cout<<"Please enter 3 arguments"<<endl;
		cout<<"quitting ..."<<endl;		
		return 0;
	}
	char server_ip[50];
	char max_clients_string[50];
	char server_port_string[50];
	int max_clients;
	unsigned short int port;	
	strcpy(server_ip,argv[1]);
	strcpy(server_port_string,argv[2]);
	strcpy(max_clients_string,argv[3]);
	max_clients = atoi(max_clients_string);
	port = atoi(server_port_string);
	cout<<server_ip<<" "<<port<<endl;
	int listenfd;
	struct sockaddr_in servaddr,client_addr;	
	
	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0))<0)
	{
		perror("socket error");
		return 0;
	}
	
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	inet_pton(AF_INET,server_ip,&(servaddr.sin_addr));
	servaddr.sin_port=htons(port);
	
	if ((bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr)))<0)
	{
		perror("bind error");
		return 0;
	}

	if ((listen(listenfd,max_clients))<0)
	{
		perror("listen error");
		return 0;
	}

	int maxfd = listenfd;
	fd_set allset, rset;
	FD_ZERO(&allset);
	FD_ZERO(&rset);
	FD_SET(listenfd, &allset);

	for (;;)
	{
		rset = allset;
		int nready = select(maxfd+1, &rset, NULL, NULL, NULL);
		for (int i=0; i<=maxfd; i++) // we check all the socket descriptors for data
		{
			if (FD_ISSET(i,&rset))//if there is a descriptor with data
			{
				if (i==listenfd)//we got a new connection
				{
					handle_new_connection(listenfd, client_addr,allset,maxfd);

				}
				else// we got data from an existing connection
				{
					got_data(client_count, i, allset, maxfd, max_clients);
				}
			}
		}
		
	}

	return 0;

}

