//Dave Pannu
//CPSC 5510 warmup_cli.cpp 
//Oct 8, 2018
//Referenced: Beej's Guide to Network Programming
//            https://beej.us/guide/bgnet/pdf/bgnet_USLetter.pdf

#include "common.h"
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
using namespace std;


const int MIN_PORT = 10540; //Minimum Port for Dave Pannu
const int MAX_PORT = 10559;	//Maximum Port for Dave Pannu
const int MAX_ARG = 2;		//Maximum command line arguements


int main(int argc, char* argv[]) {
	int status, sockfd, bytecount;
	string argString, delimiter, tempaddress;
	struct addrinfo hints;
	struct addrinfo *servinfo;
	char sendBuffer[128];		//Send buffer
	char recieveBuffer[128];	//Recieve buffer

	//Empty out hints struct
	memset(&hints, 0, sizeof hints); 

	//Verify command line arguements
	if(argc != MAX_ARG){
		cerr << "Client: Incorrect arguement format: ./warmup_cli hostname:port\n";
		cerr << "Client: Port Range for Dave Pannu: 10540 to 10559\n";
		exit(-1);
	}

	//Take arguement and convert to string
	argString= string(argv[1]); 

	//Assign delimiter
    delimiter = ":";

    //Parse string for webaddress
    tempaddress = argString.substr(0, argString.find(delimiter)); 

	//Convert tempaddress string to char*
    const char* address = tempaddress.c_str();

   	//Delete address and delimiter from str
    argString.erase(0, 0 + tempaddress.length() + 1);

    //Remainder of string contains port number,
	//Convert string of portnumber to char*
    const char* portNum = argString.c_str();
	
    //Convert str to int of portnumber
    int verifyPort = stoi(argString);

    //Verify that port number is assigned to Dave Pannu
    if(verifyPort < MIN_PORT || verifyPort > MAX_PORT){
    	cerr <<"Client: Port Range for Dave Pannu: 10540 to 10559\n";
    	exit(-1);
    }

	hints.ai_family = AF_UNSPEC;     	// don't care IPv4 or IPv6
	hints.ai_socktype = SOCK_STREAM; 	// TCP stream sockets

	//Get address info, covert url to ip address
	status = getaddrinfo(address, portNum, &hints, &servinfo);
	if (status != 0) {
        cerr << "Client: Address Error\n";
	    exit(-1);
	}

	//Create socket and check for error
	sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol); 
	if(sockfd < 0) {
		cerr << "Client: Socket Error\n";
		exit(-1);
   	}
	
	//Connect to server, if no server running will exit program here
   	status = connect(sockfd, servinfo->ai_addr, servinfo->ai_addrlen);
   	if(status < 0){
   		cerr << "Client: Connection error\n";
   		exit(-1);
   	}
	
	//Prompt user to enter a message
	string message;
    cout << "Enter a message: ";
	getline(cin, message);

	//If message is greater than 128 ask for shorter message
	while(message.length() < 1 || message.length() > 127){
		cout << "Enter message between 1 and 127 characters: ";
		getline(cin, message);
	}

	//Fill send buffer with null
	memset(sendBuffer, '\0', sizeof(sendBuffer));

	//Copy message string to sendbuffer char array
	strcpy(sendBuffer, message.c_str());

	//Send to socket and check bytecount of message
  	bytecount = send(sockfd,  sendBuffer, sizeof(sendBuffer), 0);
  	if (bytecount != sizeof(sendBuffer)){
    	cerr << "Error sending message, exiting program\n";
  	  	exit(-1);
  	}
	
	//Fill recieve buffer with null
	memset(recieveBuffer, '\0', sizeof(recieveBuffer));

	//Recieve to socket and check bytecount of message
	bytecount = recv(sockfd, recieveBuffer, sizeof(recieveBuffer), 0);
    if (bytecount != sizeof(recieveBuffer)){
    	cerr << "Error recieving server message, closing connection\n";
    	close(sockfd);
    }
	
	//Print out recieved message
	cout << recieveBuffer << endl;

	//Close socket and exit program
	close(sockfd);
	return 0;
}

