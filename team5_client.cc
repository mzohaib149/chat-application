
#include "team5_client.h"
#include <stdio.h>

void send_join_message(int sockfd, char* username)
{
	struct SBCP_JOIN_Message  sjm;
	sjm.hdr.Vrsn_and_Type = htons(VERSION3TYPE2);
	sjm.hdr.Length = htons(strlen(username)+9);
	sjm.attr_hdr.Type = htons(2);//for username
	sjm.attr_hdr.Length = htons(strlen(username)+1);
	strcpy(sjm.Username,username);
	int buffer_size = 0;
	buffer_size = strlen(username)+ 8 + 1;
	char buffer[buffer_size];
	memset(buffer, 0, buffer_size);
	memcpy(&buffer[0], &sjm.hdr.Vrsn_and_Type, 2);
	memcpy(&buffer[2], &sjm.hdr.Length, 2);
	memcpy(&buffer[4], &sjm.attr_hdr.Type, 2);
	memcpy(&buffer[6], &sjm.attr_hdr.Length, 2);
	memcpy(&buffer[8], sjm.Username, strlen(sjm.Username));
	int bytes_sent = send(sockfd, buffer, buffer_size, 0);
}

void send_send_message(int sockfd, char* message)
{
	struct SBCP_SEND_Message  ssm;
	ssm.hdr.Vrsn_and_Type = htons(VERSION3TYPE4);
	ssm.hdr.Length = htons(strlen(message)+9);
	ssm.attr_hdr.Type = htons(4);//for message
	ssm.attr_hdr.Length = htons(strlen(message)+1);
	strcpy(ssm.Message, message);
	int buffer_size = 0;
     	 buffer_size =  8 + strlen(message)+1;
	char buffer[buffer_size];	
	memset(buffer, 0, buffer_size);
	memcpy(&buffer[0], &ssm.hdr.Vrsn_and_Type, 2);
	memcpy(&buffer[2], &ssm.hdr.Length, 2);
	memcpy(&buffer[4], &ssm.attr_hdr.Type, 2);
	memcpy(&buffer[6], &ssm.attr_hdr.Length, 2);
	memcpy(&buffer[8], ssm.Message, buffer_size);
	int bytes_sent = send(sockfd, buffer, buffer_size, 0);
 	
}

void send_idle_message(int sockfd)
{
	struct SBCP_IDLE_Message_Client sim;
	sim.hdr.Vrsn_and_Type = htons(VERSION3TYPE9);
	sim.hdr.Length = htons(4);
	int buffer_size = 4;
	char buffer[buffer_size];
	memset(buffer, 0, buffer_size);
	memcpy(&buffer[0], &sim.hdr.Vrsn_and_Type, 2);
	memcpy(&buffer[2], &sim.hdr.Length, 2);
	int bytes_sent = send(sockfd, buffer, buffer_size, 0);
 	
}

void got_data(int descriptor, bool &nak_received)
{
	char header_buffer[4];
	int nbytes = recv(descriptor, header_buffer, 4, 0); 
	struct SBCP_Header h;
	memcpy(&h.Vrsn_and_Type, &header_buffer[0], 2);
	memcpy(&h.Length, &header_buffer[2], 2);
	unsigned short int temp_vrsn_and_type = ntohs(h.Vrsn_and_Type);
	h.Vrsn_and_Type = temp_vrsn_and_type;
	unsigned short int temp_length = ntohs(h.Length);
	h.Length = temp_length;
	
	
	if (((h.Vrsn_and_Type) & MASK) == 3)//received a forward
	{
		int i = h.Length - 4;
		char payload_buffer[i];
		struct SBCP_FWD_Message sfm;
		sfm.hdr.Vrsn_and_Type = h.Vrsn_and_Type;
		sfm.hdr.Length = h.Length;
		int nbytes1 = recv(descriptor, payload_buffer, i, 0); 
		memcpy(&sfm.attr_hdr1.Type, &payload_buffer[0], 2);
		memcpy(&sfm.attr_hdr1.Length, &payload_buffer[2], 2);
		
		memcpy(&sfm.Username, &payload_buffer[4],ntohs(sfm.attr_hdr1.Length));
		int j = strlen(sfm.Username) + 4 + 1;
		memcpy(&sfm.attr_hdr2.Type, &payload_buffer[j], 2);
		memcpy(&sfm.attr_hdr2.Length, &payload_buffer[j+2], 2);
		
		memcpy(&sfm.Message, &payload_buffer[j+4], ntohs(sfm.attr_hdr2.Length));
		
		sfm.attr_hdr1.Type = ntohs(sfm.attr_hdr1.Type);
		sfm.attr_hdr1.Length =  ntohs(sfm.attr_hdr1.Length);
		sfm.attr_hdr2.Type = ntohs(sfm.attr_hdr2.Type);
		sfm.attr_hdr2.Length =  ntohs(sfm.attr_hdr2.Length);		
		cout<<sfm.Username<<" says: "<<sfm.Message<<endl; 
	}
	
	if (((h.Vrsn_and_Type) & MASK) == 7)//received an ack
	{
		//cout<<"reveived an ack"<<endl;
		int i = h.Length - 4;
		char payload_buffer[i];
		struct SBCP_ACK_Message sam;
		sam.hdr.Vrsn_and_Type = h.Vrsn_and_Type;
		sam.hdr.Length = h.Length;
		int nbytes1 = recv(descriptor, payload_buffer, i, 0); 
		memcpy(&sam.attr_hdr1.Type, &payload_buffer[0], 2);
		memcpy(&sam.attr_hdr1.Length, &payload_buffer[2], 2);
		
		memcpy(&sam.Client_Count, &payload_buffer[4], 2);

		memcpy(&sam.attr_hdr2.Type, &payload_buffer[6], 2);
		memcpy(&sam.attr_hdr2.Length, &payload_buffer[8], 2);
		
		memcpy(&sam.Username, &payload_buffer[10], ntohs(sam.attr_hdr2.Length));
		
		sam.attr_hdr1.Type = ntohs(sam.attr_hdr1.Type);
		sam.attr_hdr1.Length =  ntohs(sam.attr_hdr1.Length);
		sam.attr_hdr2.Type = ntohs(sam.attr_hdr2.Type);
		sam.attr_hdr2.Length =  ntohs(sam.attr_hdr2.Length);		
		
		cout<<"Number of Clients: "<<sam.Client_Count<<endl;
		cout<<"Usernames: "<<sam.Username<<endl;
		
	}
	
	if (((h.Vrsn_and_Type) & MASK) == 5)//received a nak
	{
		nak_received = true;
		int i = h.Length - 4;
		char payload_buffer[i];
		struct SBCP_NAK_Message snm;
		snm.hdr.Vrsn_and_Type = h.Vrsn_and_Type;
		snm.hdr.Length = h.Length;
		int nbytes1 = recv(descriptor, payload_buffer, i, 0); 
		memcpy(&snm.attr_hdr.Type, &payload_buffer[0], 2);
		memcpy(&snm.attr_hdr.Length, &payload_buffer[2], 2);
		memcpy(&snm.Reason, &payload_buffer[4],ntohs(snm.attr_hdr.Length));
		snm.attr_hdr.Type = ntohs(snm.attr_hdr.Type);
		snm.attr_hdr.Length =  ntohs(snm.attr_hdr.Length);
		cout<<snm.Reason<<endl;
		close(descriptor);
		
	}
	
	if (((h.Vrsn_and_Type) & MASK) == 8)//received an online
	{
		int i = h.Length - 4;
		char payload_buffer[i];
		struct SBCP_ONLINE_Message sonm;
		sonm.hdr.Vrsn_and_Type = h.Vrsn_and_Type;
		sonm.hdr.Length = h.Length;
		int nbytes1 = recv(descriptor, payload_buffer, i, 0); 
		memcpy(&sonm.attr_hdr.Type, &payload_buffer[0], 2);
		memcpy(&sonm.attr_hdr.Length, &payload_buffer[2], 2);
		memcpy(&sonm.Username, &payload_buffer[4],ntohs(sonm.attr_hdr.Length));
		sonm.attr_hdr.Type = ntohs(sonm.attr_hdr.Type);
		sonm.attr_hdr.Length =  ntohs(sonm.attr_hdr.Length);
		
		cout<<"User "<<sonm.Username<<" has joined chatroom"<<endl;
	}

	if (((h.Vrsn_and_Type) & MASK) == 6)//received an offline
	{
		int i = h.Length - 4;
		char payload_buffer[i];
		struct SBCP_OFFLINE_Message sofm;
		sofm.hdr.Vrsn_and_Type = h.Vrsn_and_Type;
		sofm.hdr.Length = h.Length;
		int nbytes1 = recv(descriptor, payload_buffer, i, 0); 
		memcpy(&sofm.attr_hdr.Type, &payload_buffer[0], 2);
		memcpy(&sofm.attr_hdr.Length, &payload_buffer[2], 2);
		memcpy(&sofm.Username, &payload_buffer[4],ntohs(sofm.attr_hdr.Length));
		sofm.attr_hdr.Type = ntohs(sofm.attr_hdr.Type);
		sofm.attr_hdr.Length =  ntohs(sofm.attr_hdr.Length);
		
		cout<<"User "<<sofm.Username<<" has left chatroom"<<endl;
	}
	
	if (((h.Vrsn_and_Type) & MASK) == 9)//received an idle
	{
		int i = h.Length - 4;
		char payload_buffer[i];
		struct SBCP_IDLE_Message_Server sim;
		sim.hdr.Vrsn_and_Type = h.Vrsn_and_Type;
		sim.hdr.Length = h.Length;
		int nbytes1 = recv(descriptor, payload_buffer, i, 0); 
		memcpy(&sim.attr_hdr.Type, &payload_buffer[0], 2);
		memcpy(&sim.attr_hdr.Length, &payload_buffer[2], 2);
		memcpy(&sim.Username, &payload_buffer[4],ntohs(sim.attr_hdr.Length));
		sim.attr_hdr.Type = ntohs(sim.attr_hdr.Type);
		sim.attr_hdr.Length =  ntohs(sim.attr_hdr.Length);
		
		cout<<"User "<<sim.Username<<" is idle"<<endl;
	}

	
}

int main(int argc, char** argv)
{
	if (argc!=4)
	{
		cout<<"Please enter 3 arguments"<<endl;
		cout<<"quitting ..."<<endl;		
		return 0;
	}
	char username[50];	
	char server_ip[50];
	char server_port_string[50];
	if (strlen(argv[1]) > 16)
	{
		cout<<"Username too large"<<endl;
		cout<<"Please use 16 charachter or short name"<<endl;
	}
	unsigned short int server_port;	
	strcpy(username,argv[1]);
	strcpy(server_ip,argv[2]);
	strcpy(server_port_string,argv[3]);
	server_port = atoi(server_port_string);
	
	int sockfd;
	if ((sockfd = socket(AF_INET,SOCK_STREAM,0))<0)
	{
		perror("socket error");
		return 0;
	}
	struct sockaddr_in server_addr;
	bzero(&server_addr,sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(server_port);
	inet_pton(AF_INET,server_ip,&(server_addr.sin_addr));
	if ((connect(sockfd,(struct sockaddr*) &server_addr, sizeof(server_addr)))<0)
	{
		perror("connect error");
		return 0;
	}
	bool nak_received = false;
	bool chatroom_full = false;
	bool username_exists = false;
	send_join_message(sockfd, username);
	got_data(sockfd, nak_received);//check for ACK or NAK
	if (nak_received)
 		return 0;
	
	char message[512];
	cout<<"Welcome to chatroom"<<endl;
	cout<<"Joined with the server"<<endl;
	cout<<"Please enter your message"<<endl;
	
	
	
	fd_set allset, rset;
	FD_ZERO(&allset);
	FD_ZERO(&rset);
	FD_SET(0, &allset);	
	FD_SET(sockfd, &allset);
	
	while(1)
	{
		rset = allset;
		fflush(NULL);
		select(sockfd+1, &rset, NULL, NULL, NULL);//check socket for input data
		if (FD_ISSET(sockfd,&rset))
		{
			bool nack_received = false;
			got_data(sockfd, nack_received);
			if (nak_received)
				return 0;
		}

		if (FD_ISSET(0,&rset))//check console for input data
		{
			fflush(NULL);
			char message_to_send[512];
			cin.getline(message_to_send,512);
			send_send_message(sockfd, message_to_send);
			
		}
		
	}
	
	return 0;
}


