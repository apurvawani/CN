import java.io.BufferedWriter;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.net.Socket;
import java.util.ArrayList;
import java.util.Scanner;

public class SR_Receiver {
	static Socket ss1;
	static DataInputStream din;
	static DataOutputStream dout;
	static Scanner sc = new Scanner(System.in);
	public static void main(String[] args) throws IOException {
		try{
			System.out.println("Searching for Receiver ... ");
			ss1 = new Socket("localhost",7777);
			System.out.println("!!!.....Connection Established.....!!!");
			din = new DataInputStream(ss1.getInputStream());
			dout = new DataOutputStream(ss1.getOutputStream());
		}
		catch(Exception e){
			e.printStackTrace();
		}
		int winSize = din.readInt();
		try { 
			while(true){
				String ch;
				boolean[] recData = new boolean[winSize]; 
				ArrayList<String> frame = new ArrayList<String>();
				int flag = 0,nack = -1;
				din.readInt();
				System.out.println("Enter 'y' to send all acknowledgment successfully and 'n' to cause loss of acknowledgement :");
				ch = sc.next();
				if(ch.compareTo("n") == 0){
					do{
						System.out.println("Enter acknowledgment to lose :");
						nack = sc.nextInt();
						nack -= 1;
						recData[nack]=false;
						if(nack<0||nack>winSize-1){
							System.out.println("Invalid acknowledgment number. Enter again.");
						}
					}while(nack<0||nack>winSize-1);
				}

				BufferedWriter out = new BufferedWriter( 
						new FileWriter("Received.txt", true));
				for(int i=0;i<winSize;i++){
					String st = din.readUTF();
					int pno = din.readInt();
					st = st + "\n";

					if(pno != i){
						dout.writeInt(-1);
						boolean test = new Boolean(false);
						recData[i] = test;
						frame.add(st);
					}

					else{
						if(i != nack){
							boolean test = new Boolean(true);
							recData[i] = test;
							System.out.println("Frame No. : "+ (i+1) +" received.");
							dout.writeInt(i+1);
						}
						else{
							boolean test = new Boolean(false);
							recData[i] = test;
							System.out.println("Acknowledgement "+(i+1)+ " lost.");
							dout.writeInt(-1);
						}
						frame.add(st);
					}
				}

				for(int i=0;i<winSize;i++){
					boolean test = recData[i];
					if(test){
						String st = din.readUTF();
						int temp = din.readInt();
						out.write(frame.get(i));
						dout.writeInt(i+1);
					}
					else{
						int pno;
						do{
							System.out.println("Lost frame received!");
							String st = din.readUTF();
							pno = din.readInt();
							st = st + "\n";
							if(pno != i){
								pno = -1;
								dout.writeInt(-1);
							}
							else{
								out.write(st);
								dout.writeInt(i+1);
							}
						}while(pno == -1);
					}
				}
				out.close();
				System.out.println();
				//System.out.println("Recevied frame : "+frame);
			}
		} 
		catch (IOException e) { 
			System.out.println("Data received successfully!"); 
		} 
		ss1.close();
	}
}
