/*
Write a program in C/C++ to analyze following packet formats captured through Wireshark for wired network. 
1. Ethernet 
2. IP 
3. TCP 
4. UDP
*/
#include <iostream>
#include <fstream>
#include <iomanip>      //The header <iomanip> is part of the Input/output library of the C++ Standard Library. It defines the manipulator functions resetiosflags() , setiosflags() , setbase() , setfill() , setprecision() , and setw()
#include <string>
using namespace std;

int main(){
	cout << "Packet Analyzing" << endl; // prints !!!Hello World!!!
		string value, srno,time,source,destination,info,protocol,len,choice;
		int count=0,ch;
		do{
		count=0;
		cout<<"Enter choice protocol for want to analyze the packets : "<<endl;
		cout<<"1)Ethernet \n2)IP \n3)TCP \n4)UDP \n5)Exit"<<endl;
		cin>>ch;

		switch(ch){
		case 1:{
			choice="ARP";
			break;
		}
		case 2:{
			choice="ICMPv6";
			break;
		}
		case 3:{
			choice="TCP";
			break;
		}
		case 4:{
			choice="UDP";
			break;
		}
		case 5:
			break;

		default:cout<<"Enter right choice"<<endl;
		}

		ifstream f("file.csv",ios::in); // inputstream -> reading , outputstrream -> writing
		if(f){
			while(f.good()){
				getline(f,srno,','); //GET STRING TILL ,
				getline(f,time,',');
				getline(f,source,',');
				getline(f,destination,',');
				getline(f,protocol,',');
				getline(f,len,',');
				getline(f,info,'\n');
				protocol=string(protocol,1,protocol.length()-2); 
				if(protocol==choice && ch!=5){
					cout<<"Time : "<<time<<endl;
					cout<<"Source : "<<source<<endl;
					cout<<"Destination : "<<destination<<endl;
					cout<<"Protocol : "<<protocol<<endl;
					cout<<"Length : "<<len<<endl<<endl;

					count++;
				}
			}
			if(ch!=5){
			cout<<"Count of "<<choice<<" packets is : "<<endl;
			cout<<count<<endl;}
		}else{
			cout<<"Error reading file"<<endl;
		}



		}while(ch!=5);

	string s,d,b;
	cout<<"Enter source to check"<<endl;
	getline(cin,b);	
	getline(cin,s);
	cout<<"Enter destination to check"<<endl;
	getline(cin,d);

	ifstream file("file.csv",ios::in);
	if(file){
		count=0;
		while(file.good()){

						getline(file,srno,','); //GET STRING TILL ,
						getline(file,time,',');
						getline(file,source,',');
						getline(file,destination,',');
						getline(file,protocol,',');
						getline(file,len,',');
						getline(file,info,'\n');
						source=string(source,1,source.length()-2);
						destination=string(destination,1,destination.length()-2);

						if(source==s && destination==d){
							cout<<"Time : "<<time<<endl;
							cout<<"Source : "<<source<<endl;
							cout<<"Destination : "<<destination<<endl;
							cout<<"Protocol : "<<protocol<<endl;
							cout<<"Length : "<<len<<endl<<endl;

							count++;
						}
					}
					cout<<"Count of packets is : "<<endl;
					cout<<count<<endl;
	}else{
		cout<<"Error opening file"<<endl;
	}

	return 0;
}
