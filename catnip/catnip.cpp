#include "UDPSocket.h"
#include "TCPSocket.h"
#include "MulticastSocket.h"
#include <stdio.h>
#include <unistd.h>


using namespace std;

//=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=--
//		DRIVERS
//=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=--
//#define WITH_TEST_DRIVER 
#define WITH_SHELL_DRIVER 


#ifdef WITH_TEST_DRIVER
	#include <fstream>
	#include <iostream>
	#include <sys/wait.h>

	//#define TEST1 // getters & setters
		//#define TEST1_1 //HTTP GET
	//#define TEST2 //TCP Listener
	//#define TEST2_1 //TCP file receive
	//#define TEST2_2 //TCP file send
		//#define TEST3 //UDP sender
	//#define TEST4 //UDP send/receive
	#define TEST5 //Multicast send/receive
#endif


#ifdef WITH_TEST_DRIVER
//Test driver
int main(int argc, char **argv)
{
	printf("Testing the Sockets library ...\n");

	//=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=--
	//			TCP
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=--
	//create
	//struct SocketOpts opts;
	//TCPSocket *tcp = (TCPSocket*)Socket::create(Socket::TCP, opts);


#ifdef TEST1
	// Test1: test tcp client getters/setters
	//-----------------------
	TCPSocket *tcp;
	TCPSocket *tcp2;
	char *all; //make sure to free!
	char *part; //make sure to free!
	printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
	printf("Running Test1 ...\n");

	try{
		tcp = new TCPSocket();
		tcp2 = new TCPSocket();

		tcp->bind(9999, "192.168.3.5");
		//printf("Bound to %s at port %d\n", tcp->getBoundAddr().c_str(), tcp->getBoundPort());
		if(tcp->getBoundAddr() != "192.168.3.5")
		{
			printf("TEST 1 FAILED! Incorrect bound address.\n");
			return -1;
		}
		if(tcp->getBoundPort() != 9999)
		{
			printf("TEST 1 FAILED! Incorrect bound port.\n");
			return -1;
		}

		tcp2->bind(9998);
		//printf("Bound to %s at port %d\n", tcp2->getBoundAddr().c_str(), tcp2->getBoundPort());
		if(tcp2->getBoundAddr() != "0.0.0.0")
		{
			printf("TEST 1 FAILED! Incorrect bound address.\n");
			return -1;
		}
		if(tcp2->getBoundPort() != 9998)
		{
			printf("TEST 1 FAILED! Incorrect bound port.\n");
			return -1;
		}


		delete tcp;
		delete tcp2;
			
	} catch(int e) {
		printf("Error - TCP socket failed: %s\n", strerror(e));
		return -1;
	}
	printf("Test 1 PASSED!\n");
#endif


#ifdef TEST1_1
	// Test1.1: test tcp client
	//-----------------------
	TCPSocket *tcp1_1;
	char *h1_1; //make sure to free!
	printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
	printf("Running Test1_1 ...\n");

	try{
		//tcp = (TCPSocket*)Socket::create(Socket::TCP);
		tcp1_1 = new TCPSocket();
		tcp1_1->connect("127.0.0.1", 80);
		tcp1_1->send("GET /index.html HTTP/1.1\r\n\r\n");
		int allSize = tcp->receive(&h1_1);

		printf("%s\n", h1_1);

		delete tcp1_1;
		free(h1_1);
	
			
	} catch(int e) {
		printf("Error - Test 1_1 failed: %s\n", strerror(e));
		return -1;
	}
	printf("Test 1_1 PASSED! !\n");
#endif


#ifdef TEST2
	//Test2: Test TCP listener
	//-----------------------
	printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
	printf("Running Test 2\n");
	try{
		char *h2;
		int cld, status;

		if((cld = fork()) == 0)
		{
			TCPSocket tcp2;
			tcp2.bind(9997);
			tcp2.listen();
			TCPSocket *newtcp = tcp2.accept();
			newtcp->receive(&h2);
			printf("%s\n", h2);
	
			free(h2);
			delete(newtcp);

			exit(0);

		}
		else
		{
			sleep(1);
			TCPSocket tcp2sender;
			tcp2sender.connect("127.0.0.1", 9997);
			tcp2sender.send("Test 2 PASSED! TCP sender/receiver.\n");

			//waitpid(cld, &status, 0);
			
		}
	
	} catch(int e) {
		printf("Error - TCP Listener socket failed: %s\n", strerror(e));
		return -1;
	}

#endif

#ifdef TEST2_1
	//Test2.1: Test TCP File Receive
	//-----------------------
	printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
	printf("Running Test 2_1\n");
	char *h2_1;
	int h2_1Size;
	try{
		TCPSocket tcp2_1;
		tcp2_1.bind(9999);
		tcp2_1.listen();
		TCPSocket *tcp2_1_receiver = tcp2_1.accept();
		//h2_1Size = tcp2_1_receiver->receive(&h2_1);

		//write to file
    	//::ofstream qFile("Test_2_1.pdf", ios::out | ios::binary);
		//qFile.write(h2_1, h2_1Size);
		//qFile.close();

    	::ofstream qFile("Test_2_1.pdf", ios::out | ios::binary);
		//::streambuf *sbuf = qFile.rdbuf();
		//::ostream out(sbuf);
		h2_1Size = tcp2_1_receiver->receive(qFile);
		//h2_1Size = tcp2_1_receiver->receive(::cout);

		

		//printf("%s\n", h2_1);

		free(h2_1);
		delete(tcp2_1_receiver);
	
	} catch(int e) {
		printf("Error - TCP File Transfer failed: %s\n", strerror(e));
		return -1;
	}
	printf("Test 2_1 Success!\n");
#endif

#ifdef TEST2_2
	//Test2.2: Test TCP File Send
	//-----------------------
	printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
	printf("Running Test 2_2\n");
	char *h2_2;
	int h2_2Size;
	try{
		TCPSocket tcp2_2;
		tcp2_2.connect("192.168.3.56", 9998);
		::ifstream sendFile;
		sendFile.open("/tmp/Test.pdf", ios::in | ios::binary);
		tcp2_2.send(sendFile);
	
	} catch(int e) {
		printf("Error - TCP File Transfer failed: %s\n", strerror(e));
		return -1;
	}
	printf("Test 2_2 Success!\n");
#endif
		//tcp->setopt(opts);
		//tcp->getopt();
		//tcp->getStats->print();

	//get stats
		//avg RTT? use SO_TIMESTAMP
		//uptime?
		//avg throughput?
	//struct socketStats stats = tcp_sock->getStats();

	//SockOpts management ??

	

	//=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=--
	//					UDP
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=--

	//Test3: Test UDP sender
	//-----------------------
#ifdef TEST3
	printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
	printf("Running Test 3\n");
	try{

		UDPSocket udp1;
		udp1.send("Hello!\n", "192.168.3.56", 9999);

	} catch(int e) {
		printf("Error - UDP socket sender failed: %s\n", strerror(e));
		return -1;
	}
	printf("Test 3 Success!\n");
#endif

#ifdef TEST4
	//Test4: Test UDP receiver
	//-----------------------
	printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
	printf("Running Test 4 ...\n");
	char *h4;
	try{
			//UDPSocket udp2;
			//udp2.bind(9999);
			//udp2.receive(&h4);
			//printf("%s\n", h4);

		if(fork() == 0)
		{
			sleep(1);
			UDPSocket udp1;
			udp1.send("TEST 4 PASSED! UDP sender/receiver", "127.0.0.1", 9997);
			return 0;
		}
		else
		{
			UDPSocket udp2;
			udp2.bind(9997);
			udp2.receive(&h4);
			printf("%s\n", h4);

			//int status;
			//wait(&status);
		}

	} catch(int e) {
		printf("Error - UDP socket receiver failed: %s\n", strerror(e));
		return -1;
	}
#endif

#ifdef TEST5
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=--
	//					MULTICAST
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=--
	try{
		printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
		printf("Running Test 5\n");
		char *h5;
		string maddr = "224.0.0.1";
		int mport = 8564;

		//create multicast sockets
		MulticastSocket mcast;
		MulticastSocket mcast_sender;
	
		//Manage Membership: see `man 7 ip`
		//subscribe multicast
		mcast.subscribe(maddr.c_str());	
		mcast.bind(mport);	

		if(fork() == 0)
		{
			//send message
			sleep(1);
			mcast_sender.send("TEST 5 PASSED! Multicast sender/receiver", maddr.c_str(), mport);
			return 0;
	
		}
		else
		{
			//receive message
			mcast.receive(&h5);
			printf("%s\n", h5);
			free(h5);

			//int status;
			//wait(&status);
		}
	
		//mcast->unsubscribe("??");
		//mcast->getSubscriptions();
	}
	catch(int e) {
		printf("ERROR: Multicast failed - %s\n", strerror(e));
	}
#endif

}
#endif


#ifdef WITH_SHELL_DRIVER
#include <ctype.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

int main( int argc, char **argv)
{
	extern char *optarg;
	extern int optind;
    int index;
    int c;

	int file = 0;
	int listening = 0;
	int port = 0;
	int keep = 0;
	int hasMessage = 0;
	int hasAddress = 0;
	int hasPort = 0;
	int timeout = -1;
	char *address = "Not specified";
	char *message = "";
	const char *filename;
	int err;
	string helpMessage = "Usage: {tcp,udp,multi} <options> \n";
	helpMessage += "Options: \n";
	helpMessage += " -l: listen for incoming connections \n";
	helpMessage += " -k: with -l, keep the socket open for new messages \n";
	helpMessage += " -a: specify ip address for connection \n";
	helpMessage += " -f: save incoming message to file \n";
	helpMessage += " -m: specify a message to send through the socket. \n";
	helpMessage += "     Alternatively, a message may be piped in.\n";
	helpMessage += " -p: specify the port \n";
	helpMessage += " -t: specify the timeout \n";
	helpMessage += " -h: print this help message \n";
	helpMessage += "\n";


    
    opterr = 0;

   	//TODO: regex validation of optargs
	//		make address and port available as non options 
	//		broadcast option?
    while ((c = getopt (argc, argv, "f:a:p:m:t:hlk")) != -1)
	{
    	switch (c)
    	{
		  case 'h'://help
		    printf("%s\n", helpMessage.c_str());
		    return 0;
    	  case 'l'://listen
    	    listening = 1;
			//TODO: if multicast, add to subscriptions list
    	    break;
		  case 'k'://keep listening
		    keep = 1;
		    break;
		  case 'a'://address
		    address = optarg;
			hasAddress = 1;
		    break;
		  case 'f'://save to file
			file = 1;
		    filename = optarg;
		    break;
		  case 'm'://message
			hasMessage = 1;
		    message = optarg;
		    break;
		  case 'p'://port
		    port = atoi(optarg);
			hasPort = 1;
		    break;
		  case 't'://timeout
		    timeout = atoi(optarg);
		    break;
    	  case '?':
		    printf("Found an %c\n", optopt);
    	    if (optopt == 'l')
    	      fprintf (stderr, "Option -%c requires an argument.\n", optopt);
    	    else if (isprint (optopt))
    	      fprintf (stderr, "Unknown option `-%c'.\n", optopt);
    	    else
    	      fprintf (stderr,
    	               "Unknown option character `\\x%x'.\n",
    	               optopt);
    	    return 1;
    	  default:
    	    abort ();
    	}
	}

	//implements option logic
	//----------------------------
	if(keep && !listening) 
	{
		//fprintf(stderr, "ERROR: keep option is invalid if not listening.\n");
		//exit(-1);
	}
	if(listening && !hasPort)
	{
		fprintf(stderr, "ERROR: you must specify a port number.\n");
		exit(-1);
	}
	if(!listening && (!hasAddress || !hasPort))
	{
		fprintf(stderr, "ERROR: you must specify an address port number.\n");
		exit(-1);
	}
    
	//debug: print list of options
    //printf ("listening = %d\nkeep = %d\nmessage=%d\nport = %d\naddress = %s\n", listening, keep, port, message, address);
    
	//collect non-option arguments
	//----------------------------
	//TODO: allow for IP address and port to be spcified as non-option args
    for (index = optind; index < argc; index++)
	{
		printf ("Non-option argument %s\n", argv[index]);
		//string nextarg(argv[index]);
		//if(nextarg.contains(".")
	}
		
	//logic for each socket type
	//---------------------------
	string app(argv[0]);
	if( app.substr(app.length()-5) == "multi") 
	{
		 //TODO: validate port number
		MulticastSocket mcast;

		/* LISTENING MODE */
		if(listening == 1)
		{
			char *mcasthandle;
			//TODO validate this is a valid multicast address
			//TODO option to keep a list of subscriptions?
			try {
				mcast.subscribe(address);	
				mcast.bind(port);	
			} 
			catch(char* e) {
				printf("Error - Multicast socket failed: %s\n", e);
				free(mcasthandle);
				return -1;
			}

			if(keep != 1) //receive one message and quit
			{
				try{
					mcast.receive(&mcasthandle);
				}
				catch(char* e)
				{
					printf("Error - Multicast socket receive failed: %s\n", e);
					free(mcasthandle);
					return -1;
				}
				printf("%s\n", mcasthandle);
				free(mcasthandle);
			}
			else //keep listening until killed
			{
				while(1)
				{
					mcast.receive(&mcasthandle);
					printf("%s\n", mcasthandle);
					free(mcasthandle);
					//sleep(10);
				}
			}
		}
		/* SENDING MODE */
		else
		{
			if(hasMessage == 1)
				mcast.send(message, address, port);
			else if(keep == 1)
			{
				char m[1000];
				for(int i = 0; i< 1000; i++) m[i] = '\0';
				while( ::cin.getline(&m[0], 999, '\0'))
				{
					mcast.send(&m[0], address, port);
					//sleep(10);
				}
			}
			else mcast.send(::cin, address, port);
		}
	
	}
	else if( app.substr(app.length()-3) == "udp")
	{
		//TODO: validate port number
		UDPSocket udp;

		if(listening == 1)
		{
			char *udphandle;
			udp.bind(port);

			if(keep != 1) //receive one message and quit
			{
				udp.receive(&udphandle);
				printf("%s\n", udphandle);
				free(udphandle);
			}
			else //keep listening until killed
			{
				while(1)
				{
					udp.receive(&udphandle);
					printf("%s", udphandle);
					free(udphandle);
				}
			}
		}
		else
		{
			//TODO address and port mandatory

			if(hasMessage == 1)
				udp.send(message, address, port);
			else
				udp.send(::cin, address, port);

		}
		
	}
	else if(app.substr(app.length()-3) == "tcp")
	{
		//TODO: validate port number
		TCPSocket tcp;
		if(timeout > 0) tcp.setTimeout(timeout);
		if(listening == 1)
		{
			//char *handle;
			tcp.bind(port);
			tcp.listen();
			TCPSocket *newtcp; 
			::ofstream fstr;
			if(file == 1)
			{
   				fstr.open(filename, ios::out | ios::binary);
			}

			if(keep != 1) //receive one message and quit
			{
				newtcp = tcp.accept();
				if(file == 1)
				{
					newtcp->receive(fstr);
					fstr.close();
				}
				else
				{
					//printf("Starting to receive\n");
					try{
						while( newtcp->receive(std::cout, 1024*100) > 0) std::cout.flush();
						//newtcp->receive(std::cout, -1);
						std::cout.flush();
					}
					catch(char* err)
					{
						newtcp->receive(std::cout);
						std::cout.flush();
					}
				}
			
				//printf("%s", handle);
				//free(handle);
				delete(newtcp);
			}
			else //keep receiving messages until killed (or timeout)
			{
				while(1)
				{
					//TODO: can we make this infinite?
					tcp.setTimeout(1000);
					newtcp = tcp.accept();
					newtcp->receive(std::cout);
					//printf("%s", handle);
					//free(handle);
					delete(newtcp);
				}
			}
		}
		else
		{
			try
			{
				tcp.connect(address, port);
			}
			catch(char* errorMessage)
			{
				printf("ERROR %s\n", errorMessage);
				exit(-1);
			}
			

			if(hasMessage == 1)
				tcp.send(message);
			else
				tcp.send(::cin);
		}
	}
	else
	{
		printf("Unknown app mode: %s\n", app.c_str());
	}

    return 0;
}

#endif


