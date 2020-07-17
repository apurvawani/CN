#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <netinet/in.h> // The header file in.h contains constants and structures needed for internet domain addresses.
#include <string.h>
#include <unistd.h>
#include <fstream>
using namespace std;

#define PORT 8999
#define buflen 1000;


// http://www.linuxhowtos.org/C_C++/socket.htm
void die(const char *s) 
{
	perror(s); // The perror() function in C++ prints the error message to stderr
	exit(1);
}

int main() {
	
	string t;
	const char *error; // const char* cuz it shld match the datatype of parameter to die()  
	struct sockaddr_in server_addr , client_addr;
	/*
		eg . struct sockaddr_in serv_addr, cli_addr;
		So serv_addr and cli_addr are 2 instances of structure sockaddr_in
		1) A sockaddr_in is a structure containing an internet address defined in netinet/in.h.
		2) serv_addr will contain the address of the server, and cli_addr will contain the address of the client which connects to the server.
	*/
	cout<<"\nUDP SERVER \n";

	//step 1: Creating Socket
	int sock;
	if((sock=socket(AF_INET,SOCK_DGRAM,0))==-1) 	{	//AF_INET is an address family that is used to designate the type of addresses that your socket can communicate with (in this case, Internet Protocol v4 addresses). 

	/*
		The socket() system call creates a new socket. Takes three arguments. 
		1) first is the address domain of the socket. (AF_NET -> for any two hosts on the Internet)
		2) type of socket.SOCK_STREAM and SOCK_DGRAM.
		3)  protocol. If this argument is zero operating system will choose the most appropriate protocol
	
		 returns an entry into the file descriptor table . If the socket call fails, it returns -1.
	*/										
		t = "SOCKET CREATE ERROR";
		error = t.c_str(); // c_str() converts a C++ string into a C-style string as a parameter to a function that expects a C-style string  
		die(error);
	}

	/* now that socket is created assign values to all the fields in that structure for server */
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT); //converts the unsigned short integer hostshort from host byte order to network byte order.
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // to set the IPv4 address from a "string"

	//BINDING socket to port
	if(bind(sock,(struct sockaddr *)&server_addr,sizeof(server_addr))==-1) {

		/*
			1) bind() system call binds a socket to an address,
			2)  It takes three arguments, the socket file descriptor, the address to which is bound, and the size of the address to which it is bound.
		*/
		t = "BINDIND ERROR";
		error = t.c_str();
		die(error);
	}
	else
		cout<<"\nUDP SERVER STARTED!\n";

	int client_len=sizeof(client_addr);
	socklen_t cli=sizeof(client_addr);	//socklen_t->data byte count
	int choice,msg_len;
	char buffer[1000];
	bzero((char *)buffer,sizeof(buffer)); //  initializes the buffer using the bzero() 
	msg_len = recvfrom(sock,buffer,1000,0,(struct sockaddr *)&client_addr,&cli);

	/*
		int recvfrom(int socket, char *buffer, 
             int length, int flags,
             struct sockaddr *address, 
             int *address_length);

	General description
	The recvfrom() function receives data on a socket named by descriptor socket and stores it in a buffer. 
	socket
    The socket descriptor.
	buffer
	    The pointer to the buffer that receives the data.
	length
	    The length in bytes of the buffer 
	flags
   	 A parameter that can be set to 0, 
	address
	    A pointer to a socket address structure from which data is received. If address is nonzero, the source address is returned.
	address_length
	    Must initially point to an integer that contains the size in bytes of the storage pointed to by address. 	

	*/

	if(msg_len>0)
	{
		cout<<"Client :- "<<buffer<<endl;
		bzero((char *)buffer,sizeof(buffer));
		// after its use initialise buffer back to 0
	}
	do
	{
		cout<<"\nEnter your choice : \n1.SEND FILE\n2.RECEIVE FILE\n3.EXIT\n";

		cin>>choice;
		switch(choice)
		{
		case 1:
		{
			cout<<"\nEnter Filename : ";
			char filename[100];
			cin>>filename;
			fstream fout;
			fout.open(filename,ios::in|ios::out|ios::binary);
			fout.seekg(0,ios::end); // place pointer at eof
			int filesize = fout.tellg(); // get size of file
			char *filebuff = new char[filesize];
			fout.seekg(0,ios::beg); // take file pointer to start of file 
			fout.read(filebuff,filesize); 
			/*
				
              fstream fin;
              fin.read( (char *) &obj, sizeof(obj) );

The read() function takes two arguments.
&obj : Initial byte of an object stored in file.
sizeof(obj) : size of object
			*/

			msg_len=sendto(sock,filename,strlen(filename),0,(struct sockaddr *)&client_addr,client_len); // just the file name 1st

			/*
			int sendto(int socket, char *buffer, int length, int flags,
           struct sockaddr *address, int address_len);

				The sendto() function sends data on the socket with descriptor socket. The sendto() call applies to either connected or unconnected sockets.

Parameter
    Description
socket
    The socket descriptor.
buffer
    The pointer to the buffer containing the message to transmit.
length
    The length of the message in the buffer pointed to by the msg parameter.
flags
    Setting these flags is not supported in the AF_UNIX domain.
address
    The address of the target.
addr_len
    The size of the address pointed to by address. 


			*/
			if(msg_len==-1) {
				t = "Filename error";
				error = t.c_str(); 
				die(error);
			}

			msg_len=sendto(sock,(void *)&filesize,sizeof(filesize),0,(struct sockaddr *)&client_addr,client_len); // 2ndly send the file size
			if(msg_len==-1) {
				t = "Filesize error";
				error = t.c_str(); 
				die(error);
			}

			msg_len=sendto(sock,filebuff,filesize,0,(struct sockaddr *)&client_addr,client_len); // now send the actual file 
			if(msg_len==-1) {
				t = "File transmission error";
				error = t.c_str();
				die(error);
			}
			else
				cout<<"Transmission Successful!\n";
			fout.close();
			// hence we have : filename -> filesize -> actual file
		}
		break;
		case 2:
		{
			char filename[100];
			bzero((char *)filename,sizeof(filename));
			msg_len=recvfrom(sock,filename,99,0,(struct sockaddr*)&client_addr,&cli);

			/*
			int recvfrom(int socket, char *buffer, 
			     int length, int flags,
			     struct sockaddr *address, 
			     int *address_length);

			General description
			The recvfrom() function receives data on a socket named by descriptor socket and stores it in a buffer. The recvfrom() function applies to any datagram socket, whether connected or unconnected.

			Parameter
			    Description
			socket
			    The socket descriptor.
			buffer
			    The pointer to the buffer that receives the data.
			length
			    The length in bytes of the buffer pointed to by the buffer parameter. If the MSG_CONNTERM flag is set, the length of the buffer must be zero.
			flags
			    A parameter that can be set to 0
			address
	    A pointer to a socket address structure from which data is received. If address is nonzero, the source address is returned.
	address_length
	    Must initially point to an integer that contains the size in bytes of the storage pointed to by address. 
			*/
			if(msg_len==-1) {
				t = "Filename error!";
				error = t.c_str(); 
				die(error);
			}
			cout<<"\nFilename:"<<filename;

			int filesize;
			msg_len=recvfrom(sock,(void *)&filesize,sizeof(filesize),0,(struct sockaddr*)&client_addr,&cli);
			cout<<"\nFileSize:"<<filesize << endl;
			char *filebuff=new char[filesize];


			bzero((char *)filebuff,sizeof(filebuff));
			msg_len=recvfrom(sock,filebuff,filesize,0,(struct sockaddr*)&client_addr,&cli);
			ofstream fout1;
			fout1.open(filename,ios::out|ios::binary);
			if(!fout1) {
				t = "CANNOT CREATE FILE";
				error = t.c_str();
				die(error);
			}
			else
			{
				fout1.write(filebuff,filesize);
				fout1.close();
				cout<<"File received!\n";
			}
		}
			break;
		}
	}while(choice!=3);
close(sock);
	return 0;
}
