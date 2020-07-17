#include<bits/stdc++.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include <netdb.h> // needed for struct hostent 
using namespace std;

class Client{
private:
	int port, clientDesc;
	char buffer[1024];
	sockaddr_in sendSocketAddr;

public:
	Client(){
		port = 8000;
		//struct hostent* host = gethostbyname("127.0.0.1"); // It defines a host computer on the Internet.
		bzero((char*)&sendSocketAddr, sizeof(sendSocketAddr));
		sendSocketAddr.sin_family = AF_INET;
		sendSocketAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); //The function inet_ntoa() converts a network address in a struct in_addr to a dots-and-numbers format string
		sendSocketAddr.sin_port = htons(port);

		clientDesc = socket(AF_INET, SOCK_STREAM, 0);
		if(clientDesc < 0){
			cout << "Error in socket creation!" << endl;
			exit(0);
		}
		cout << "Socket created!" << endl;
	}
	

	void connectToServer(){
		int status = connect(clientDesc, (sockaddr*) &sendSocketAddr, sizeof(sendSocketAddr));
		if(status < 0){
			cout << "Can't Connect to server!" << endl;
			exit(0);
		}
		cout << "Connection established successfully!" << endl;
	}

	void chat()
	{
		while(true)
		{
			cout << "Enter Client's Message('q' to terminate chat')";
			string data;
			 //cin.ignore(numeric_limits<streamsize>::max(),'\n'); 
			getline(cin, data);
	        memset(&buffer, 0, sizeof(buffer));
	        strcpy(buffer, data.c_str()); // convert the msg to c-style string then send it to server
            send(clientDesc, (char*)&buffer, strlen(buffer), 0);
	        if(data == "q")
	        {
	        	cout << "Closing connection!" << endl;
	        	break;
	        }

	        //cout << "Waiting for server response .... " << endl;
	        memset(&buffer, 0, sizeof(buffer));
	        recv(clientDesc, (char*)&buffer, sizeof(buffer), 0);
	        cout << "Server's Message : " << buffer << endl;
	        if(!strcmp(buffer , "q")) {
	        	cout << "Closing connection!" << endl;
	        	break;
	        }
		}
		
	}

	void calc() {
		
		double result;
		char s[10];
		cout << "Enter trignometric expression : "; 
		cin >> s;
		//cout << s;
		send(clientDesc , &s , sizeof(s) , 0);
		recv(clientDesc, &result, sizeof(result), 0);
		cout << "\nOperation result from server = " << result << "\n";  
	}


	void closeConnection(){
		close(clientDesc);
	}
	void file()
	{
		ifstream fin;
		cout << "ENTER NAME OF FILE: ";
		string s;
		cin >> s;
		memset(&buffer, 0, sizeof(buffer));
		strcpy(buffer, s.c_str()); // convert the filename to c-style and then send to server
		send(clientDesc, (char*)&buffer, strlen(buffer), 0);
		fin.open(s, ios::in);
		char data[1000];
		memset(&buffer, 0, sizeof(buffer));
		while(!fin.eof()){
			getline(fin, s);
			strcpy(data, s.c_str()); // convert file content to c-style string
			strcat(buffer, data); // concatenate that c-style string to create entire buffer of c-style format
			strcat(buffer, "\n");
		}
		send(clientDesc, (char*)&buffer, strlen(buffer), 0);

		fin.close();
	}
	void menu()
	{
		int ch;
		while(true)
		{
			cout << "\n1. Chat "
				"\n2. Transfer File"
				"\n3. Calculator"
				"\n4. Exit"
				"\nEnter your choice : ";
			cin>>ch;
			cin.ignore(numeric_limits<streamsize>::max(),'\n'); 
			if( ch>=1 && ch<=4 )
				send(clientDesc, &ch, sizeof(ch), 0);
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
				default:
					cout<<"Wrong choice, enter it again !!! ";
					break;
			}
		}
	}
};

int main(){
	Client obj;
	obj.connectToServer(); // connect()
	obj.menu();
	
	return 0;
}
