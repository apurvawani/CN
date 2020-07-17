#include <bits/stdc++.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <unistd.h>

using namespace std;

#define PORT 8999
#define buflen 1000;
#define SERVER_ADDRESS "127.0.0.1"

void die(const char *s) 
{
	perror(s);
	exit(1);
}

int main() {
	const char *error;
	string t;
	struct sockaddr_in server_addr;
	cout<<"\nUDP CLIENT\n";

	
	int sock = socket(AF_INET,SOCK_DGRAM,0);
	if(sock == -1) 								
	{											
		t = "SOCKET CREATE ERROR!";
		error = t.c_str(); 
		die(error);
	}
	
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(PORT);
	server_addr.sin_addr.s_addr=inet_addr(SERVER_ADDRESS);
	
	/*  unlike server we do not have bind() here */
	socklen_t cli=sizeof(server_addr); // this is used while receiving ie for recvfrom()
	int slen = sizeof(server_addr); //  // this is used while sending ie for sendto()
	int choice,msg_len;
	char buffer[1000];
	cout<<"\nEnter msg to server : ";
	cin.getline(buffer,sizeof(buffer));
	msg_len=sendto(sock,buffer,strlen(buffer),0,(struct sockaddr *)& server_addr,slen);
	do
	{
		cout<<"\nEnter your choice : \n1.RECEIVE FILE\n2.SEND FILE\n3.EXIT\n";
		cin>>choice;
		switch(choice)
		{
		
		case 1:
		{
			char filename[100];
			bzero((char *)filename,sizeof(filename));
			msg_len=recvfrom(sock,filename,99,0,(struct sockaddr*)&server_addr,&cli);
			if(msg_len==-1) {
				t = "Filename error";
				error = t.c_str(); 
				die(error);
			}
			cout<<"\nFilename : " << filename << "\n";

			int filesize;
			msg_len=recvfrom(sock,(void *)&filesize,sizeof(filesize),0,(struct sockaddr*)&server_addr,&cli);
			cout<<"\nFilesize : "<<filesize << "\n";
			char *filebuff=new char[filesize];

			bzero((char *)filebuff,sizeof(filebuff));
			
			msg_len=recvfrom(sock,filebuff,filesize,0,(struct sockaddr*)&server_addr,&cli);
			ofstream fout;
			fout.open(filename,ios::out|ios::binary);
			if(!fout) {
				t = "CANNOT CREATE FILE!";
				error = t.c_str();
				die(error);
			}
			else
			{
				fout.write(filebuff,filesize);
				fout.close();
				cout<<"File received!\n";
			}
		}
			break;

		case 2:
		{	
			cout<<"\nEnter Filename : ";
			char filename[100];
			cin>>filename;
			fstream fout1;
			fout1.open(filename,ios::in|ios::out|ios::binary);
			fout1.seekg(0,ios::end);
			int filesize=fout1.tellg(); //get file size
			char *filebuff=new char[filesize];
			fout1.seekg(0,ios::beg);
			fout1.read(filebuff,filesize); //reading file content

			msg_len=sendto(sock,filename,strlen(filename),0,(struct sockaddr *)&server_addr,slen); //send filename
			if(msg_len==-1) {
				t = "Filename error";
				error = t.c_str(); 
				die(error);
			}

			msg_len=sendto(sock,(void *)&filesize,sizeof(filesize),0,(struct sockaddr *)&server_addr,slen); //send filesize
			if(msg_len==-1) {
				t = "Filesize error";
				error = t.c_str(); 
				die(error);
			}

			msg_len=sendto(sock,filebuff,filesize,0,(struct sockaddr *)&server_addr,slen); //send file contents
			if(msg_len==-1) {
				t = "File transmission error";
				error = t.c_str();
				die(error);
			}
			else
				cout<<"Transmission Successful!\n";
			fout1.close();
		}
						break;
		}
	}while(choice!=3);

	close(sock);
	return 0;
}
