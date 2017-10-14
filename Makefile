all: client server clean

client: team5_client.o
	g++ team5_client.o -o client

server: team5_server.o
	g++ team5_server.o -o server

team5_client.o: team5_client.cc team5_client.h team5_common.h
	g++ -c team5_client.cc

team5_server.o: team5_server.cc team5_server.h team5_common.h
	g++ -c team5_server.cc

clean: 
	rm *.o
