import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.ArrayList;
import java.util.Scanner;

public class GBN_Sender {
	static ServerSocket ss;
	static Socket ss1;
	static DataInputStream din;
	static DataOutputStream dout;
	static Scanner sc = new Scanner(System.in);
	GBN_Sender(){

	}
	public static void main(String[] args) throws IOException {
		try{
			System.out.println(" HEY! Receiver Server Starting... Plz wait...");
			ss = new ServerSocket(7221);    //create a server socket bound to the specified port
			System.out.println("Waiting For the Client to be connected ... ");
			ss1 = ss.accept();			
			System.out.println("Client Connected !!! ");
			din = new DataInputStream(ss1.getInputStream());
			dout = new DataOutputStream(ss1.getOutputStream());
		}
		catch(Exception e){
		}

		Data s = new Data();
		int winSize , seqF , seqN , m , size;
		seqF = 0;
		seqN = 0;
		System.out.println("Enter m which represents bit representation : ");
		m = sc.nextInt();
		winSize = (int) (Math.pow(2,m)-1);
		System.out.println("Window size is : " + winSize);
		seqN = winSize;
		dout.writeInt(winSize);
		//System.out.println("try block starting");
		try{
			while(true){
				seqN = seqF + winSize;
				if(seqF == s.data.size()){
					break;
				}
				ArrayList<String> frame = null;
				//System.out.println("Before making frame");
				frame = s.makeFrame(seqF, seqN);
				System.out.println("Window : " + frame);
				int i , nsend = -1;
				String ch;
				System.out.println("Enter y to send all frames successfully and n to cause loss of frame : ");
				ch = sc.next();
				if(ch.compareTo("n") == 0){
					do{
						System.out.println("Enter frame number to lose : ");
						nsend = sc.nextInt();
						nsend -= 1;
						if(nsend < 0 || nsend > winSize-1){
							System.out.println("Invalid frame number! Enter again : ");
						}
					}while(nsend < 0 || nsend > winSize-1);
				}
				dout.writeInt(0);
				if(frame.size() < winSize) {
					size = frame.size();
				}
				else {
					size = winSize;
				}
				for(i = 0 ; i < size ; i++) {
					//System.out.println("Sender " + i);
					if(i == nsend){
						System.out.println("Frame lost ("+(nsend+1)+ ")");
						dout.writeUTF("#");
						dout.writeInt(-1);
					}
					else{
						dout.writeUTF(frame.get(i));
						//System.out.println(frame.get(i));
						dout.writeInt(i);
						//System.out.println("i : " + i);
					}
					int ack = din.readInt();
					if(ack == ((i) + 1)){
						System.out.println("Acknowledgement for frame " + ack + " received.");
						seqF++;
					}
				}
				//System.out.println("Sender side for loop finished");
				nsend = -1;
				System.out.println();
			}
		}
		catch(Exception e){
			System.out.println();
		}
		ss.close();
		ss1.close();
	}

}
