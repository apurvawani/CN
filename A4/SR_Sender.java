import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.ArrayList;
import java.util.Scanner;

public class SR_Sender {
	static ServerSocket ss;
	static Socket ss1;
	static DataInputStream din;
	static DataOutputStream dout;
	static Scanner sc = new Scanner(System.in);
	public static void main(String[] args) throws IOException {
		try{
			System.out.println(" HEY! Receiver Server Starting... Plz wait...");
			ss = new ServerSocket(7777);
			System.out.println("Waiting For the Client to be connected ... ");
			ss1 = ss.accept();
			System.out.println("Client Connected !!! ");
			din = new DataInputStream(ss1.getInputStream());
			dout = new DataOutputStream(ss1.getOutputStream());
		}
		catch(Exception e){
		}
		Data s = new Data();
		int winSize,seqF,seqN,m=0;
		seqF = 0;
		seqN = 0;
		System.out.println("Enter m which represents bit representation : ");
		m=sc.nextInt();
		winSize = (int) (Math.pow(2,m)-1);
		System.out.println("Window size is : "+winSize);
		
		seqN = winSize;
		dout.writeInt(winSize);
		//s.getData();
		try{
			while(true){
				seqN = seqF + winSize;
				if(seqN == s.data.size()){
					break;
				}
				ArrayList<String> frame = null;
				boolean[] recAck = new boolean[winSize]; 
				frame = s.makeFrame(seqF, seqN);
				System.out.println("Window : "+frame);
				int i,nsend = -1,nrec = -1;
				String ch;
				System.out.println("Enter 'y' to send all frames successfully and 'n' to cause loss of frame : ");
				ch = sc.next();
				if(ch.compareTo("n") == 0){
					do{
						System.out.println("Enter frame to lose : ");
						nsend = sc.nextInt();
						nsend -= 1;
						if(nsend<0||nsend>winSize-1){
							System.out.println("Invalid frame number.Enter again.");
						}
					}while(nsend<0||nsend>winSize-1);
				}
				dout.writeInt(0);
				for(i=0;i<winSize;i++){
					if(i == nsend){
						dout.writeUTF("#");
						dout.writeInt(-1);
					}
					else{
						dout.writeUTF(frame.get(i));
						dout.writeInt(i);
					}
					int ack = din.readInt();
					if(ack != (i + 1)){
						boolean test = new Boolean(false);
						recAck[i] = test;
					}
					else{
						System.out.println("Acknowledgement for frame no. "+ack+" received.");
						boolean test = new Boolean(true);
						recAck[i] = test;
					}
				}
				for(i=0;i<winSize;i++){
					boolean test = recAck[i];
					if(test){
						dout.writeUTF("#");
						dout.writeInt(-1);
						int k = din.readInt();
					}
					else{
						do{
							System.out.println("Resending Frame No. " + (i+1));
							dout.writeUTF(frame.get(i));
							dout.writeInt(i);
							nrec = din.readInt();
							if(nrec != i+1){
								nrec = -1;
							}
						}while(nrec == -1);
						nrec = -1;
					}
				}
				nsend = -1;
				System.out.println();
				seqF = seqF + winSize;
			}
		}
		catch(Exception e){
			System.out.println("Data Transmission done!");
		}
		ss.close();
		ss1.close();
	}
}
