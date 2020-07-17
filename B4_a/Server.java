import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.SocketException;

public class Server {
	private ServerSocket severSocket = null;
	private Socket socket = null;
	private InputStream inStream = null;
	private OutputStream outStream = null;

	public Server() {

	}

	public void createSocket() {
		try {
			ServerSocket serverSocket = new ServerSocket(3339);
			while (true) {
				socket = serverSocket.accept();
				inStream = socket.getInputStream();
				outStream = socket.getOutputStream();
				System.out.println("Connected!");
				createReadThread();
				createWriteThread();
			}
		} 
		catch (IOException io) {
			io.printStackTrace();
		}
	}

	public void createReadThread() {
		Thread readThread = new Thread() {
			public void run() {
				while (socket.isConnected()) {
					try {
						byte[] readBuffer = new byte[200];
						int num = inStream.read(readBuffer);    //The java.io.InputStream.read() method reads the next byte of the data from the the input stream and returns int in the range of 0 to 255. If no byte is available because the end of the stream has been reached, the returned value is -1.
						if (num > 0) {
							byte[] arrayBytes = new byte[num];
							System.arraycopy(readBuffer, 0, arrayBytes, 0, num);   
							String recvedMessage = new String(arrayBytes, "UTF-8");   //Construct a new String by decoding the byte array. It uses the char_set for decoding.
							System.out.println("Received message :" + recvedMessage);
						}
						else {
							notify();  // wakes up a single thread that is waiting on this object's monitor. If many threads are waiting on this object, one of them is chosen to be awakened. The choice is arbitrary and occurs at the discretion of the implementation.
						};
						//System.arraycopy();

					} 
					catch (SocketException se) {
						System.exit(0);
					} 
					catch (IOException i) {
						i.printStackTrace();
					}

				}
			}
		};
		readThread.setPriority(Thread.MAX_PRIORITY);
		readThread.start();
	}

	public void createWriteThread() {
		Thread writeThread = new Thread() {
			public void run() {

				while (socket.isConnected()) {
					try {
						BufferedReader inputReader = new BufferedReader(new InputStreamReader(System.in));
						sleep(100);
						String typedMessage = inputReader.readLine();
						if (typedMessage != null && typedMessage.length() > 0) {
							synchronized (socket) {
								outStream.write(typedMessage.getBytes("UTF-8"));     //In order to convert a String into UTF-8, we use the getBytes() method in Java. The getBytes() method encodes a String into a sequence of bytes and returns a byte array.
								sleep(100);
							}
						}/* else {
					notify();
					}*/
					;
					} 
					catch (IOException i) {
						i.printStackTrace();
					} 
					catch (InterruptedException ie) {
						ie.printStackTrace();
					}

				}
			}
		};
		writeThread.setPriority(Thread.MAX_PRIORITY);
		writeThread.start();

	}

	public static void main(String[] args) {
		Server chatServer = new Server();
		chatServer.createSocket();

	}
}
