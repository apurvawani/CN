/*
IMP Links: 
http://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.html
*/


#include <bits/stdc++.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
using namespace std;

class Server{
private:
	char buffer[1024];        //The server reads characters from the socket connection into this buffer. 
	string serverIP;
	int port, serverDesc, socketDesc;
	sockaddr_in serverAddr, socketAddr;          //basic structure for all syscalls and functions that deal with internet addresses
	socklen_t socketSize;						//<sys/socket.h> makes available a type, socklen_t, which is an unsigned opaque integral type of length of at least 32 bits.

public:
	Server(){
		port = 8000;
		socketDesc = 0;
		bzero((char*)&serverAddr, sizeof(serverAddr)); //The function bzero() sets all values in a buffer to zero. It takes two arguments, the first is a pointer to the buffer and the second is the size of the buffer.
		serverAddr.sin_family = AF_INET; //AF_INET is an address family that is used to designate the type of addresses that your socket can communicate with (in this case, Internet Protocol v4 addresses). 
		serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); //host to network long
		// IP address of the machine on which the server is running, and there is a symbolic constant INADDR_ANY which gets this address. 
		serverAddr.sin_port = htons(port); //the function htons() which converts a port number in host byte order to a port number in network byte order. 
		serverDesc = socket(AF_INET, SOCK_STREAM, 0);
		//  socket() system call creates a new socket.
		// 1) address domain of the socket
		// 2) type of socket -> SOCK_STREAM and SOCK_DGRAM. 
		// 3) protocol. If this argument is zero operating system will choose the most appropriate protocol(TCP or UDP)
		//   If the socket call fails, it returns -1
		if(serverDesc < 0){
			cout << "Error in socket creation" << endl;
			exit(0);
		}
		cout << "Socket Created!" << endl;
		socketSize = sizeof(socketAddr);
	}
	
	void connectClient(){
		int bindStatus = bind(serverDesc, (struct sockaddr*) &serverAddr, sizeof(serverAddr));
		if(bindStatus < 0){
			cout << "UnSuccessful binding!" << endl;
			exit(0);
		}
		cout << "Waiting for client ... " << endl;

		listen(serverDesc, 3); // absent in udp 
		//To listen for up to 3 requests
		//the listen system call allows the process to listen on the socket for connections. The first argument is the socket file descriptor, and the second is the size of the backlog queue, i.e., the number of connections
		socketDesc = accept(serverDesc, (sockaddr *)&socketAddr, &socketSize);
		// it wakes up the process when a connection from a client has been successfully established. It returns a new file descriptor, and all communication on this connection should be done using the new file descriptor.
		if(socketDesc < 0){
			cout << "Error while accepting request from client!" << endl;
			exit(0);
		}
		cout << "Successfully connected with client!" << endl;
	}

	void chat(){
		while(true){
			//cout << "Awaiting client response" << endl;
			memset(&buffer, 0, sizeof(buffer));
			recv(socketDesc, (char*)&buffer, sizeof(buffer), 0); // udp has recvfrom , here it is just recv 
			// https://docs.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-recv
			if(!strcmp(buffer, "q")){
				cout << "Client has Quit the Chat!!!" << endl;
				break;
			}

			cout << "Client's Message : " << buffer << endl;
			string data;
			cout << "Server Message('q' to terminate chat)";
			getline(cin, data);
	        memset(&buffer, 0, sizeof(buffer));
	        strcpy(buffer, data.c_str());
        	send(socketDesc, (char*)&buffer, sizeof(buffer), 0); // udp has sendto , here it is send 
	        if(data == "q"){
				cout << "Server has Quit the Chat!!!" << endl;
				break;
	        }
		}
		
	}

	void calc()
	{
		double result , val , PI = 3.14159;
		char s[10];
		recv(socketDesc , &s , sizeof(s) , 0);
		//cout << s;
		char *p , *x;
		p = strstr(s , "sin"); // do man
		// char *strstr(const char *haystack, const char *needle);
		//  The  strstr() function finds the first occurrence of the substring needle in the string haystack.
		// These functions return a pointer to the beginning of the  located  substring, or NULL if the substring is not found.
		if(p) {
			x = s+3;
			//cout << x;
			val = atoi(x);
			result = sin(val*PI/180); // sin gives wrong ans with sin(val)
		}
		p = strstr(s , "cos");
		if(p) {
			x = s+3;
			val = atoi(x);
			result = cos(val);
		}
		p = strstr(s , "tan");
		if(p) {
			x = s+3;
			val = atoi(x);
			result = tan(val);
		}
		p = strstr(s , "cosec");
		if(p) {
			x = s+5;
			val = atoi(x);
			result = 1/sin(val);
		}
		p = strstr(s , "sec");
		if(p) {
			x = s+3;
			val = atoi(x);
			result = 1/cos(val);
		}
		p = strstr(s , "cot");
		if(p) {
			x = s+3;
			val = atoi(x);
			result = 1/tan(val);
		}
        
        cout << "\nResult sent to client!" << endl;
        send(socketDesc, &result , sizeof(result) , 0);

	}

	void closeConnection(){
		close(socketDesc);
		close(serverDesc);
	}
    void file()
    {
        fstream fout;
        memset(&buffer, 0, sizeof(buffer));
        recv(socketDesc, (char*)&buffer, sizeof(buffer), 0); // 1st filename is received 
        fout.open(buffer, fstream::out);
        memset(&buffer, 0, sizeof(buffer));  
        recv(socketDesc, (char*)&buffer, sizeof(buffer), 0); // Then file content is received 
        fout << buffer << endl;
        cout << "File Received!\n Transfer Successfully!\n" << endl;
        fout.close();
    }
	void menu()
	{
		int ch;
		

		while(true)
		{
			recv(socketDesc, &ch , sizeof(ch), 0);

			switch(ch)
			{
				case 1:
					chat();
					break;
				case 2:
					file();
					break;
				case 3:
					calc();
					break;
				case 4:
					closeConnection();
					exit(0);
					break;
				default:cout<<"Wrong choice, enter it again !!! ";
				    break;
			}
		}
	}
};

int main(){
	Server obj;
	obj.connectClient(); // bind() , listen() and accept()
	obj.menu();
	return 0;
}
