if 0 {
	1)OTcl usually refers to an object oriented extension of Tcl 
	2)The “$” sign is used to obtain a value contained in a variable
	3)square brackets are an indication of a command substitution.
	    eg)	set a 8
		set b [expr $a/8] 
	4) the command $ns is used for using the functions belonging to the simulator class that  has member functions which enables to create the nodes and define the links between them.
	5) c++ (implementation of protocols) and OTCL (user interface)
	6) TCP -> needs acknowledgement while UDP-> does not care about acknowledgements hence we get small standing lines moving from 3 to 2 to 1 to give ack packets
	7) Simulator is a C++ class and ns is its object

}

#Create a simulator object
set ns [new Simulator]

#Define different colors for data flows (for NAM)
$ns color 1 Blue
$ns color 2 Red

if 0 {
	1) nf, nf1 are objects to open files and read them  
}

#create animation information or NAM file creation
set nf [open out.nam w]
$ns namtrace-all $nf

if 0 {
	1) $ accesses the entire op file by using $nf1 and traces it using trace-all function then assigns the output to ns object of Simulator class 
}

#Create a trace file , this file is for logging purpose
set nf1 [open out.tr w]
$ns trace-all $nf1

#Define a 'finish' procedure
proc finish {} {
	# whenever we open a file we shld close it and these ns nf nf1 file name variables to be accessed here we shld make them global 
        global ns nf
	# flush the log files
        $ns flush-trace
        #Close the NAM trace file
        close $nf
        #Execute NAM on the trace file
        exec nam out.nam &
        exit 0
}

#Create four nodes
set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]
set n3 [$ns node]
set n4 [$ns node]

#Create links between the nodes (bitrate , delay time , name of the queue)
#Drop tail means dropping the tail in case of queue overflow
$ns duplex-link $n0 $n2 2Mb 10ms DropTail
$ns duplex-link $n1 $n2 2Mb 10ms DropTail
$ns duplex-link $n2 $n3 1.7Mb 20ms DropTail
$ns duplex-link $n3 $n4 5.0Mb 20ms DropTail

#Set Queue Size of link (n2-n3) to 10
$ns queue-limit $n2 $n3 10

#Give node position (for NAM)
$ns duplex-link-op $n0 $n2 orient right-down
$ns duplex-link-op $n1 $n2 orient right-up
$ns duplex-link-op $n2 $n3 orient right
$ns duplex-link-op $n3 $n4 orient right-up

#Monitor the queue for link (n2-n3). (for NAM)
$ns duplex-link-op $n2 $n3 queuePos 0.5


#Setup a TCP connection
set tcp [new Agent/TCP]
$tcp set class_ 2
$ns attach-agent $n0 $tcp
set sink [new Agent/TCPSink]
$ns attach-agent $n4 $sink
$ns connect $tcp $sink
$tcp set fid_ 1

#Setup a FTP over TCP connection
#FTP - File Transfer Protocol (eg . To download a file from a network)
set ftp [new Application/FTP]
$ftp attach-agent $tcp
$ftp set type_ FTP


#Setup a UDP connection
# Agent/UDP is a pre-compiled library available in NS2
set udp [new Agent/UDP]
$ns attach-agent $n1 $udp
set null [new Agent/Null]
$ns attach-agent $n3 $null
# virtual connection between udp and null
$ns connect $udp $null
$udp set fid_ 2

#Setup a CBR over UDP connection
#CBR - Constant Bit Rate (eg mp3 files that have a CBR or 192kbps , 320kbps, etc..) 
set cbr [new Application/Traffic/CBR]
$cbr attach-agent $udp
$cbr set type_ CBR
$cbr set packet_size_ 1000
$cbr set rate_ 1mb
$cbr set random_ false


#Schedule events for the CBR and FTP agents and time is in sec
$ns at 0.1 "$cbr start"
$ns at 1.0 "$ftp start"
$ns at 4.0 "$ftp stop"
$ns at 4.5 "$cbr stop"

#Detach tcp and sink agents (not really necessary)
$ns at 4.5 "$ns detach-agent $n0 $tcp ; $ns detach-agent $n3 $sink"

#Call the finish procedure after 5 seconds of simulation time
$ns at 5.0 "finish"

#Print CBR packet size and interval
puts "CBR packet size = [$cbr set packet_size_]"
puts "CBR interval = [$cbr set interval_]"

#Run the simulation puts helps to cout statements on terminal
puts "Simulation starts............"
$ns run

