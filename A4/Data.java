import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;

public class Data {
	ArrayList<String> data = new ArrayList<String>();
	Data()
	{		
		try { 
			BufferedReader in = new BufferedReader( 
					new FileReader("data.txt")); 
			String getstring; 
			while ((getstring = in.readLine()) != null) {   //A String containing the contents of the line, not including any line-termination characters, or null if the end of the stream has been reached.
				data.add(getstring);
			}
			in.close();
		} 
		catch (IOException e) { 
			System.out.println(""); 
		} 
	}
	public ArrayList<String> makeFrame(int start,int end){
		ArrayList<String> frame = new ArrayList<String>();
		//System.out.println("Inside makeFrame function");
		if(end > data.size()) {
			end = data.size();
		}
		for(int i = start ; i < end ; i++) {
            //System.out.println("Inside for");
			frame.add(data.get(i));
		}
		//System.out.println("Exiting from makeFrame");
		return frame;		
	}
}
