import java.io.BufferedWriter;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.net.Socket;
import java.util.Scanner;

public class GBN_Receiver {
	static Socket ss1;
	static DataInputStream din;
	static DataOutputStream dout;
	static Scanner sc = new Scanner(System.in);
	public static void main(String[] args) throws IOException, InterruptedException {
		try{
			System.out.println("Searching for Receiver ... ");
			ss1 = new Socket("localhost",7221);
			System.out.println("!!!.....Connection Established.....!!!");
			din = new DataInputStream(ss1.getInputStream());
			dout = new DataOutputStream(ss1.getOutputStream());
		}
		catch(Exception e){
		}
		int winSize = din.readInt();
		try { 
			while(true){
				String ch;
				int flag = 0,nack = -1;
				din.readInt();
				System.out.println("Enter y to send all acknowledgment successfully and n to cause loss of acknowledgement :");
				ch = sc.next();
				if(ch.compareTo("n") == 0){
					do{
						System.out.println("Enter acknowledgment to lose : ");
						nack = sc.nextInt();
						nack -= 1;
						if(nack < 0 || nack > winSize-1){
							System.out.println("Invalid acknowledgment number! Enter again : ");
						}
					}while(nack < 0 || nack > winSize-1);
				}
				
				BufferedWriter out = new BufferedWriter( 
						new FileWriter("Received.txt", true));
				//System.out.println("Receiver side for loop begin");
				for(int i = 0 ; i < winSize ; i++) {
					//System.out.println("Recv " + i);
					String st = din.readUTF();
					int pno = din.readInt();
					st = st + "\n";					
					if(pno != i ){
						System.out.println("Frame no. : "+ (i+1) +" not received.");
						flag = 1;
					}
					if(i==nack && flag!=1) {
						System.out.println("Acknowledgement no. : "+ (i+1) + " lost.");
						flag=1;
					}
					if(flag != 1){
						dout.writeInt(i+1);
						out.write(st);
					}
					else{
						dout.writeInt(-1);
					}

				}
				System.out.println("For loop finished!");
				flag = 0;
				out.close();
			}
			//System.out.println("Exiting try block");
		} 

		catch (IOException e) { 
			System.out.println("Data Transfer Successful!");
			//System.out.println("Exception Occoured : " + e); 
		} 
		ss1.close();
	}
}
