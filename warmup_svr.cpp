//Dave Pannu
//CPSC 5510 warmup_svr.cpp 
//Oct 8, 2018
//Referenced: Beej's Guide to Network Programming
//            https://beej.us/guide/bgnet/pdf/bgnet_USLetter.pdf
#include "common.h"
#include <iostream>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <algorithm>
#include <arpa/inet.h>
using namespace std;


const int MIN_PORT = 10540;	//Minimum Port for Dave Pannu
const int MAX_PORT = 10559;	//Maximum Port for Dave Pannu
const int MAX_PENDING = 5;  //Max connections
const int MAX_ARGS = 2;     //Max command line arguements
const int PORT_ARG = 1;     //Command line arg location for Port#




int main(int argc, char** argv) {
	int status, portNum, sock, sockfd, bytecount;
  	struct sockaddr_storage client_addr;
  	socklen_t addr_size;
	struct addrinfo hints;           
	struct addrinfo *servinfo;            
  	char recieveBuffer[128];  //Recieve buffer
  	char sendBuffer[128];     //Send buffer

	//Empty out hints struct
	memset(&hints, 0, sizeof hints); 
 	
  	//Verify that port number is included with command line arguement
	if (argc != MAX_ARGS){
    	cerr << "Server: Incorrect arguement. Format:./warmup_svr port\n";
    	cerr << "Server: Port Range for Dave Pannu: 10540 to 10559\n";
    	exit(-1);
  	}

	//Set portNum to commandline entered port
	portNum = atoi(argv[PORT_ARG]);

  	//If port number is less than or greater than range for student, exists program
  	if (portNum < MIN_PORT || portNum > MAX_PORT) {
    	cout <<"Port number is not assigned to this program\n";
    	cout <<"Enter port range between "<<MIN_PORT<<" and "<<MAX_PORT<<endl;
    	exit(-1);
  	}

	hints.ai_family = AF_UNSPEC;     // Either IPv4 or IPv6
	hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
	hints.ai_flags = AI_PASSIVE;     // Fill in server IP
	
  	//Get address information into linked list
  	status = getaddrinfo(NULL, argv[PORT_ARG], &hints, &servinfo);
  	if(status != 0){
    	cerr << "Client: Address Error\n";
    	exit(-1);
	}

  	//Create socket and check for error
	sock = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
	if (sock < 0) {
    	cerr << "Error with socket" << endl;
    	exit (-1);
  	}

  	//Bind socket and check for error
	status = bind(sock, servinfo->ai_addr, servinfo->ai_addrlen);
	if (status < 0){
    	cerr << "Error with bind" << endl;
    	exit(-1);
  	}

  	//Listen on socket and check for error
	status = listen(sock, MAX_PENDING);
	if (status < 0) {
    	cerr << "Error with listen.\n";
    	close(sock);
    	exit (-1);
  	}
    
  	//Keep server running after client disconnects  
  	while(true){
    	//Accept socket connection
    	addr_size = sizeof(client_addr);
    	sockfd = accept(sock, (struct sockaddr *)&client_addr, &addr_size);
    	if (sockfd < 0){
      		cerr << "Error with accept";
      		exit(-1);
    	}
    
    	//Fill recieve buffer with NULL
    	memset(recieveBuffer, '\0', sizeof(recieveBuffer));

    	//Recieve message from client and check size of message
    	bytecount = recv(sockfd, recieveBuffer, sizeof(recieveBuffer), 0);
    	if (bytecount != sizeof(recieveBuffer)){
    		cerr << "Error recieving client message, closing connection\n";
      		close(sockfd);
    	}

	    //Print out Client message
	    cout <<"Server Recieved: "<< recieveBuffer << endl;

	    //Reverse string
	    reverse(recieveBuffer, recieveBuffer + strlen(recieveBuffer));

	    //Fill send buffer with NULL
		memset(sendBuffer, '\0', sizeof(sendBuffer));

	    //Copy reversed message to send buffer
	    strcpy(sendBuffer, recieveBuffer);

	    //Send message to client, verify correct size
	    bytecount = send(sockfd, sendBuffer, sizeof(sendBuffer), 0);
	    if (bytecount != sizeof(sendBuffer)){
	    	cerr <<"Error sending message, closing connection\n";
      		close(sockfd);
    	}

	    //Close socket
	    close(sockfd);

	}
	
	//Close socket
	close(sockfd);

    return 0;
}

