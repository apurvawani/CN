/*
Write a program  in Java/Python to demonstrate sub netting and find the sub net masks.
*/
import java.util.*;
import java.io.*;
import java.net.*;

public class subnet {
	Scanner sc = new Scanner(System.in); 
    String ip , subnetmk;	
    int baseIPnumeric;
    int netmaskNumeric;
    
    static String appendZeroes(String s)
    {
	    String temp = new  String("00000000");
	    return temp.substring(s.length())+ s;
    }

	void subnetmask()throws Exception
	{
		DataInputStream dis = new DataInputStream(System.in);
        System.out.println("Enter IP Address (eg: 192.168.1.1)");
        String ipAddr = dis.readLine();
        ip = ipAddr;
        String[] ipAddrParts = ipAddr.split("\\.");
        String mask = "";
		//default subnet mask
        int firstoctet = Integer.parseInt(ipAddrParts[0]);
        if(firstoctet<=127)
        {
                        mask = "255.0.0.0";
                        System.out.println("\nThis address is Class A IP Address.");
                        System.out.println("The Default Subnet mask is : " + mask);
        }
        else if(firstoctet>=128 && firstoctet<=191)
        {
                        mask = "255.255.0.0";
                        System.out.println("This address is Class B IP Address.");
                        System.out.println("The Default Subnet mask is : " + mask);
        }
        else if(firstoctet>=192 && firstoctet<=223)
        {
                        mask = "255.255.255.0";
                        System.out.println("This address is Class C IP Address.");
                        System.out.println("The Default Subnet mask is : " + mask);
        }
        else if(firstoctet>=224 && firstoctet<=239)
        {
                        mask = "255.0.0.0";
                        System.out.println("This address is Class D IP Address. Used for multicasting!");
        }
        else if(firstoctet>=240 && firstoctet<=254)
        {
                        mask = "255.0.0.0";
                        System.out.println("Class E IP Address. Experimental Use!");
        }

       
    System.out.println();
   
    System.out.println("\n-----------------------------------------------------");
    
		

		//----- Finding the Subent mask-----

        
      //Split the ip address
      		String split_ip[] = ipAddr.split("\\.");
      		//Converting the ip into binary
      		String split_bip[] = new String[4];
      		String bip = "";
      		//into binary form
        
        for(int i = 0 ; i < 4 ; i++) {
			split_bip[i] = appendZeroes(Integer.toBinaryString(Integer.parseInt(split_ip[i])));
			bip += split_bip[i];
		}
		System.out.println("\nThe binary IpAddress is : " + bip);
		System.out.println();
		System.out.println("\n-----------------------------------------------------");
		System.out.println("\nHow many address you want in each subnet : ");
		int n = sc.nextInt();
        System.out.println("--------------------------------------");
		int bits = (int)Math.ceil(Math.log(n)/Math.log(2));
		System.out.println("\nNumber of bits for host id = " + bits);

		int mask1 = 32-bits;
		int total_address = (int)Math.pow(2,bits);
		//System.out.println("Calculated Subnet mask address is : " + mask1);

		
		//---------------------------------------------
		int addr = 0;
		for(int i = bits ; i <= 7 ; i++) {
			addr = addr + (int)Math.pow(2, i);
		}
		
		//System.out.println("\nSubnet mask : " + (int)addr);
		
		int total_add = (int)Math.pow(2,bits);
		if(mask1>=0 && mask1<=8)
		{
			subnetmk = addr + "0.0.0";
		}
		else if(mask1>8 && mask1<=16)
		{
			subnetmk = "255." + addr + ".0.0";
		}
		else if(mask1>16 && mask1<=24)
		{
			subnetmk = "255.255" + addr + ".0";
		}
		else if(mask1>24 && mask1<=32)
		{
			subnetmk = "255.255.255." + addr;
		}
		System.out.println("Subnet mask is = " + subnetmk);
		//---- Finding the first and last address----

		//---- First address Calculation--------
		int fbip[]=new int[32];

		for(int i=0;i<32;i++)
		{
		//Convert to the character 1,0 to integer 1,0

		fbip[i]=(int)bip.charAt(i)-48;

		}

		for(int i=31;i>31-bits;i--)
		{
		//Get first address by anding the last bits with 0

		fbip[i] &=0;
		}

		String fip[]={"","","",""};
		for(int i=0;i<32;i++)
		{
		fip[i/8]=new String(fip[i/8]+fbip[i]);
		} 
		int first_offset=0;
		int ipAddr1[]=new int[4];  	
		System.out.println("------------------------------------");
		System.out.println("Group 1 \nThe First Address is:");
		for(int i=0;i<4;i++)
		{
		System.out.print(ipAddr1[i]=first_offset=Integer.parseInt(fip[i],2));
		if(i!=3)
		System.out.print(".");
		}
		System.out.println();


		//--- Last address Calculation----
		int lbip[]=new int [32];

		for(int i=0;i<32;i++)
		{
		// Convert the character 1,0 to integer 1,0

		lbip[i]=(int)bip.charAt(i)-48;
		}

		for(int i=31;i>31-bits;i--)
		{

		// Get last address by oring with last bits with 1

		lbip[i]|= 1;
		}
		 String lip[]={"","","",""};
		for(int i=0;i<32;i++)
		{
		lip[i/8]=new String(lip[i/8]+lbip[i]);
		}
		int ipLast[]=new int[4]; 
		System.out.println("\nThe Last Address is:");
		for(int i=0;i<4;i++)
		{
		System.out.print(ipLast[i]=Integer.parseInt(lip[i],2));
		if(i!=3)
		System.out.print(".");
		}
		
		
		System.out.println("\n_____________________________________");
		System.out.println();
		int scount=(int)Math.pow(2,bits);
		int n1=256/scount;
		
		for(int j=1;j<n1;j++)
		{
			System.out.println(" GROUP: "+ (j+1)+"\n\n FIRST ADDRESS:");
			for(int i=0;i<4;i++)
			{
				if(i<3)
				{
				System.out.print(ipAddr1[i]+".");
				}
				else
				System.out.println(ipAddr1[i]=ipAddr1[i]+total_address);
			}
			System.out.println( "\n LAST ADDRESS:");
			for(int i=0;i<4;i++)
			{
				if(i<3)
				{
				System.out.print(ipLast[i]+".");
				}
				else
				System.out.println(ipLast[i]=ipLast[i]+total_address);

			}
			System.out.println("\n_____________________________________");
		}

	}
	
	
	public static void main(String args[])throws Exception
	{	Scanner sc = new Scanner(System.in); 
	subnet s = new subnet();
	
		int ch;
		do{
			System.out.println("\n-----------Menu------------\n");
			System.out.println("1.Calculate the subnet mask");
			System.out.println("2.Ping");
			System.out.println("3.Exit");
			System.out.println("Enter your choice:");
			ch=sc.nextInt();
			switch(ch)
			{
			case 1:s.subnetmask();
			       break;
			
			case 2:{
				System.out.println("Enter interface for assigning IP : ");
				String ifc = sc.next();
				try {
					Process pr = Runtime.getRuntime().exec("sudo ifconfig " + ifc + " " + s.ip + " netmask " + s.subnetmk);
				} catch (IOException e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				}
				System.out.println("IP assigned successfully!");
				int f = 0;
				String ping;
				do
				{
					System.out.println("Enter IP address to ping: ");
					ping = sc.next();
					String[] ip_x = ping.split("\\.");
					if(ip_x.length != 4)
					{
						f = 1;
						System.out.println("Invalid IP!");
					}
					else
					{
						f = 0;
						int[] ipx = new int[4];
						for(int i = 0; i < 4; i++)
						{
							ipx[i] = Integer.parseInt(ip_x[i]);
							if(ipx[i] > 255 || ipx[i] < 0)
							{
								System.out.println("Error in " + "octet " + (i+1));
								System.out.println(ipx[i] + " does not lie between 0 and 255");
								f = 1;
							}
						}
					}
				}
				while(f == 1);
				try
				{
					//adr=sc.nextLine();
					Process p1=java.lang.Runtime.getRuntime().exec("ping -c 5 " + ping);
					/*BufferedReader reader=new BufferedReader(new InputStreamReader(p1.getInputStream()));
					String line="";
					while((line=reader.readLine())!=null)
					{
						System.out.println(line);
					}*/
					int returnVal = p1.waitFor();
					if(returnVal == 0)
						System.out.println("\nPinged successfully");
					else if(returnVal == 2)
						System.out.println("\nHost Unreachable!");
					else if(returnVal == 256)
						System.out.println("\nHost is Shutdown!");
				}
				catch(Exception e)
				{
					System.out.println(e);
				}
				
			}
			       break;
			case 3:System.out.println("\nThank you.....!");
			       
			}
			
						
		}while(ch!=3);
	}

}
