BEGIN {
	# initializing variables first
    recvdSize = 0
    transSize = 0
    startTime = 400
    stopTime = 0
}

{
# IN AWK , $1 refers to 1st column  , $2 refers to 2nd col and so on
    event = $1
    time = $2
    send_id = $3
    rec_id = $4
    pkt_size = $6
    flow_id = $8

    # Store start time
    # startTime is initialised by 400 initially
    # ----------------------------------------------------sending of packets-------------------------------------------
    if (send_id == "0") {
        if (time < startTime) {
            startTime = time
        }

        if (event == "+") {
            # Store transmitted packet's size
            transSize += pkt_size
        }
    }

    # Update total received packets' size and store packets arrival time
    # ----------------------------------------------------receival of packets-------------------------------------------
    if (event == "r" && rec_id == "3") {
        if (time > stopTime) {
            stopTime = time
        }
        # Store received packet's size
        if (flow_id == "1") {
            recvdSize += pkt_size
        }
    }
}

END {
	# display the results in END section 
	# we can use C syntax here 
    printf("%i\t%i\t%.2f\t%.2f\n", transSize, recvdSize, startTime, stopTime)
}
