CS 640 - Programming Assignment 3
===================================

Due: Thursday December 9, 11:00 PM

Overview
--------

In this programming assignment, you will modify the emulators that you implemented in project 2, to perform a link-state routing protocol to determine the shortest paths between a fixed, known set of nodes in the lab. The paths between the nodes will be reconfigurable and new routes must stabilize within a fixed time period.

Your emulators will also forward packets from the routetrace application that you will build to the node which is the next hop in the shortest path to a specified destination.

In this assignment, we will not provide you with each and every detail of how you should implement the link-state protocol. Instead, we will specify a set of requirements that your program should satisfy.

As with the first two programming assignments, you are to work in teams and write your code in C or C++. You will demo your code in the CS lab 1350.

Project Details
---------------

Each node in the network is defined by an {IP,port} pair. After start up, each emulator will implement the following functions: readtopology, createroutes, and forwardpacket .

readtopology
------------

readtopology will read a text file which defines the interconnection structure of a test network that can have up to 20 nodes. The topology structure will be stored in a file and will have the following format:


IP_a,port_w IP_b,port_x IP_c,port_y IP_d,port_z ... 
IP_b,port_x IP_a,port_w IP_c,port_y IP_d,port_z ... 
.
.
.
The first IP,port pair in each line of the topology file corresponds to a node which is running an emulator and will be listening for packets from all of the remaining IP,port pairs in the line (ie. a one-way connection to the first node from all of the other nodes). You can assume that there will be bidirectional connections to and from each node in the topology and that the topology file will be set up to reflect this. A simple example is as follows:


		       2 ----- 5			
		     /   \      \			
		   /      \      \			
		 /         \      \		
		1------------3----4		
					
		Node IP      Port
		1    1.0.0.0  1
		2    2.0.0.0  2
		3    3.0.0.0  3
		4    4.0.0.0  4
		5    5.0.0.0  5 
							
		Topology.txt

		1.0.0.0,1 2.0.0.0,2 3.0.0.0,3
		2.0.0.0,2 1.0.0.0,1 3.0.0.0,3 5.0.0.0,5
		3.0.0.0,3 1.0.0.0,1 2.0.0.0,2 4.0.0.0,4
		4.0.0.0,4 3.0.0.0,3 5.0.0.0,5
		5.0.0.0,5 2.0.0.0,2 4.0.0.0,4

The assumption after readtopology is executed is that all nodes should be "alive" and that the process of setting up the routing table should begin. readtopology only needs to be executed once when the emulator is started and the application can assume that the topology file is in the same directory in which the emulator code is running. Note that this means that the emulator knows the whole topology at the startup.

createroutes
-------------

createroutes should implement a link-state routing protocol to set up shortest path routes between nodes in the specified topology. You should refer to the course textbook for details on how the link-state protocol works and follow that protocol in your implementation.

The createroutes function should run continuously after the topology has been specified by the readtopology. It must be designed to react to nodes being responsive or unresponsive in the network and will require link-state information to be transmitted between an emulator and its neighbors.

The interval of transmission (ie. how frequently updates are sent) is up to you as is the mode of transport (TCP or UDP) and the link-state packet format. However you must insure that your routing topology stabilizes within at least 5 seconds after a node state change takes place (For example when emulator 3 is disconnected). For the purpose of the routing algorithm, you should assume that the distance between neighbor nodes is 1 ie. weights on each link between nodes is 1.

Notes
Note that you should implement the link-state protocol. Thus, you should implement the "reliable flooding" algorithm where each node communicates only with its neighbors. It is true that your emulator can figure out every node on the topology from the topology file but it is NOT OK to contact any node other than your neighbors directly in this implementation.
Note that your shortest path computations should be updated both when a node goes down, and when a node comes up.
forwardpacket

forwardpacket will determine where to forward a packet received by an emulator in the network. Your emulator should be able to handle both packets regarding the link-state protocol, and packets that are forwarded to it from the routetrace application. The packet format of the link-state messages is up to you.

Emulator

The emulator will be invoked as follows:

 		emulator -p <port> -f <filename>
port is the port that the emulator listens on for incoming packets.
filename is the name of the topology file described above.
Note: You might want to print some debugging information on the emulator so that if your program is not behaving as expected at the demo time we can analyze what your program does and does not do correctly.

routetrace Details
------------------

routetrace is an application similar to the standard routetrace which will trace the hops along a shortest path between the source and destination emulators. routetrace will send packets to the source emulator with successively larger time-to-live values until the destination node is reached and will produce an output showing the shortest path to the destination. You will use this application to verify that your implementation of link-state protocol has the correct shortest paths between the nodes.

This application will generate an output that traces the shortest path between the source and destination node in the network that is given to it by the command line parameters below. An instance of routetrace will be invoked as follows:

 trace -a <routetrace port> -b < source hostname> -c <source port> -d <destination hostname>
-e <destination port> -f <debug option>
routetrace port is the port that the routetrace listens on for incoming packets.
routetrace will output the shortest path between the <source hostname, source port> to <destination hostname, destination port> .
When the debug option is 1, the application will print out the following information about the packets that it sends and receives: TTL of the packet and the src. and dst. IP and port numbers. It will not do so when this option is 0.
This is the suggested packet format for the routetrace application:

 --------------------------------------------------------------------------------------------
 | char | unsigned long | unsigned long | unsinged short | unsigned long   | unsigned short | 
 | Type	|  TTL          | src IP address|  src port      | dest. IP address| dest. port     | 
 --------------------------------------------------------------------------------------------
More concretely here is what the routetrace application does:

It gets the source and destination IP and port from the command line.
It sets the TTL to 0
Sends a routetrace packet to the source with packet fields: "T", TTL, routetrace IP, routetrace Port, Destination IP, Destination Port
Waits for a response.
Once it gets a response prints out the responders IP and port (that it gets from the response packet).
If the source IP and port fields of the routetrace packet that it received equals the destination IP and port that it received from the command line then TERMINATES.
Otherwise, TTL = TTL + 1, goto 3.
Here is what your emulator should do once it receives a routetrace packet:

If TTL is 0, create a routetrace packet or modify the received routetrace packet. Put its own IP and Port to the source IP and port fields of the routetrace packet. Other fields of the packet should be identical to the packet it received. Send that back to the routetrace (using the source IP and port fields of the routetrace packet that it received)
If TTL is not 0, decrement the TTL field in the packet. Send the altered packet to the next hop on the shortest path to the destination.
Changes from Project 2

There will be no sender or requester involved in this project. Moreover, note that the emulator will not be tested for functions such as queuing, logging, loss percentage or delay. The emulator will read the topology file and not the table containing the routing information from project 2. It is sufficient that the emulator supports the functions described above.

Testing
--------

The Ctrl+ C command on the terminal will be used to temporarily disable an emulator in the topology. The idea is that the topology must be reconfigurable on the fly. When an emulator is disabled, it will cease forwarding packets and cease sending its routing messages to its neighbors. When the emulator is started again, it will begin participating in routing and forwarding again and the shortest path routes will get updated.

Sample test case:

Consider the above topology. If we run the routetrace application between nodes 1 and 4, here is the output that you should get:

Hop#  IP, Port
1     1.0.0.0, 1
2     3.0.0.0, 3
3     4.0.0.0, 4
Now lets disable emulator 3 by using the command Ctrl + C. Your routes should reconfigure. Once we run the routetrace application again after a few seconds, we should get:

Hop#  IP, Port
1     1.0.0.0, 1
2     2.0.0.0, 2
3     5.0.0.0, 5
4     4.0.0.0, 4
Your program will be tested similarly with another topology at the demo time.

Submission/Demo
---------------

The executable program names must be "emulator" and "trace". You also must supply a makefile with your source code to produce the binaries and also "clean" them when necessary. Only submit the absolute necessary files. Do not submit topology files or binaries.

To turn in your code, copy your code to the directory

 ~cs640-1/handin/<CS_USERNAME>/p3
Each group is required to sign up for a time slot to demo their work. TAs will inform you of the available demo schedule. Demos will be performed in room 1350. You are responsible to make sure that your code works on these machines before you hand the code in (ssh to these machines and test your code even if you are working in another computer lab).

Grading
--------

Grading will be according to the following schedule:

Points	Requirement
10	You have implemented the readtopology function and each emulator knows its neighbors.
30	You have implemented the createroutes function to perform a link-state protocol after the nodes come up.
15	You have implemented the forwardpacket function to forward packets from the routetrace application and other emulators.
25	You have correctly implemented the routetrace application.
20	Shortest path routes that routetrace prints are correct and reconfigure when emulators are disabled or enabled after they are disabled.
