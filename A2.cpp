/*
Write a program in C/C++ for error detection and correction for 7/8 bits ASCII codes using Hamming Codes or CRC. 
Demonstrate the packets captured traces using Wireshark Packet Analyzer Tool for peer to peer mode.
*/

#include <bits/stdc++.h>
using namespace std;

#define endl "\n"

class HammingCode {
    int *data;
	int *send;
	int *recv_data;
public :
	int m , r;
	HammingCode(char c) {
	    r = 0;
		int temp[32];		
		int n = c , t = 0;
		cout << endl << "Ascii Value : " << n << endl;
		while(n > 0) {
		    temp[t] = n % 2;
			n /= 2;
			t++;		
		}
		m = t;
		data = new int[m+1];
		cout << endl << "Data Word is : ";
		for(int i = 1 ; i <= m ; i++) {
			data[i] = temp[m-i];
			cout << data[i];
		}
		cout << endl;
		while((m+r+1) >=  pow(2 , r)) {
			r++;
		}
		cout << endl << "Redundant bits required : " << r << endl;
		recv_data = new int[m+r+1];
		send = new int[m+r+1];
		int x = 0 , y = 1;
		for(int i = 1 ; i <= m+r ; i++) {
			if(i == pow(2 , x)) {
				recv_data[i] = 0;
				x++;
			}
			else {
				recv_data[i] = data[y];
				y++;
			}
		}
	}
	void display(bool flag);
	void parityBit(int choice);
	void correct(int choice);
	void insertError(int error);
};

void HammingCode :: display(bool flag) {
	if(flag == true) {
		int x = 0;
		for(int i = 1 ; i <= m+r ; i++) {
			if(i == pow(2 , x)) {
				cout << "P" << i << "	";
				x++;
			}
			else
				cout << "D" << i << "	";
		}
		cout << endl;
		for(int i = 1 ; i <= m+r ; i++) {
			cout << recv_data[i] << "	";
		}
		cout << endl;
	}
	else {
		int x = 0 , val = 0 , k = m-1;
		for(int i = 1 ; i <= m+r ; i++) {
			if(i == pow(2 , x))
				x++;
			else {
				cout << recv_data[i];
				val += pow(2 , k) * recv_data[i];
				k--;
			}
		}
		char p = val;
		cout << endl << "Message : " << p;
		cout << endl;
	}
}

void HammingCode :: parityBit(int choice) {
	//Even Parity
	int x = 0 , min , max , p , k , j;
	if(choice == 1) {
		for(int i = 1 ; i <= m+r ; i = pow(2 , x)) {
			++x;
			p = 0;
			k = i;
			min = 1;
			max = i;
			for(j = i ; j <= m+r ; j = k+i) {
				for(k = j ; max >= min && k <= m+r ; ++min , ++k) {
					if(recv_data[k] == 1)
						p++;
				}
				min = 1;
			}
			if(p % 2 == 0)
				recv_data[i] = 0;
			else 
				recv_data[i] = 1;
		}
	}
	//Odd Parity
	else if(choice == 2){
		for(int i = 1 ; i <= m+r ; i = pow(2 , x)) {
            x++;
            p = 0;
            k = i;
            min = 1;
            max = i;
            for(j = i ; j <= m+r ; j = k+i) {
                for(k = j ; max >= min && k <= m+r ; min++ , k++) {
                    if(recv_data[k] == 1)
                        p++;
                }
                min = 1;
            }
            if(p % 2 == 0)
                recv_data[i] = 1;
            else
                recv_data[i] = 0;
        }
	}
	else {
		cout << "Invalid Choice!" << endl;
		return;
	}
}

void HammingCode :: insertError(int error) {
	recv_data[error] ^= 1;
}

void HammingCode :: correct(int choice) {
	vector <int> err;
	//Even Parity
	int x = 0 , min , max , p , k , j;
	if(choice == 1) {
		for(int i = 1 ; i <= m+r ; i = pow(2 , x)) {
			++x;
			p = 0;
			k = i;
			min = 1;
			max = i;
			for(j = i ; j <= m+r ; j = k+i) {
				for(k = j ; max >= min && k <= m+r ; ++min , ++k) {
					if(recv_data[k] == 1)
						p++;
				}
				min = 1;
			}
			if(p % 2 == 0) {
				err.push_back(0);
			}
			else {
				err.push_back(1);
			}
		}
	}
	//Odd Parity
	else if(choice == 2) {
		for(int i = 1 ; i <= m+r ; i = pow(2 , x)) {
			x++;
			p = 0;
			k = i;
			min = 1;
			max = i;
			for(j = i ; j <= m+r ; j = k+i) {
				for(k = j ; max >= min && k <= m+r ; min++ , k++) {
					if(recv_data[k] == 1)
						p++;
				}
				min = 1;
			}
			if(p % 2 == 0) {
				err.push_back(1);
			}
			else {
				err.push_back(0);
			}
		}
	}
	else {
		cout << "Invalid Choice!" << endl;
		return;
	}
	int val = 0 , i = err.size()-1;
	while(i >= 0 ) {
		val = val + (pow(2 , i)*err[i]);
		i--;
	}
	if(val != 0) {
		cout << endl << "Error detected at bit position " << val << " ( ";
		for(int i = err.size()-1 ; i >= 0 ; i--) {
			cout << err[i];
		}
		cout << " )" << endl;
	}
	recv_data[val] ^= 1;
}

int main() {
	cout << "*************** SENDER ***************" << endl;
	char s;
	cout << "Enter character input : ";
	cin >> s;
	HammingCode hc(s);
	int choice , error;
	cout << endl << "Enter your choice : " << endl;
	cout << "1. Even Parity" << endl;
	cout << "2. Odd Parity" << endl;
	cin >> choice;
	hc.parityBit(choice);
	cout << endl << "Code Word is : " << endl; 
	hc.display(true);
	cout << endl << "Do you want to add error? If yes enter the bit number else enter zero : ";
	cin >> error;
	if(error > (hc.m + hc.r))
		cout << "Invalid error bit!" << endl;
	else {
		cout << endl << "*************** RECEIVER ***************" << endl;
		cout << endl << "Codeword recvived  is : " << endl;
		hc.insertError(error);
		hc.display(true);
		if(error != 0) {
			hc.correct(choice);
			cout << "Error is corrected!" << endl;
			cout << endl << "Corrected Codeword is : " << endl;
			hc.display(true);
		}
		cout << endl << "Data Word is : ";
		hc.display(false);
	}
	return 0;
}
