#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include "Socket.h"

/***************************************************************************
 *
 *	TCPSocket 
 * ------------------
 *
 *	Author: Terrance Medina
 *
 *	class TCPSocket: 
 *
 *	provides a TCP socket for use in network applications.
 *
 *
 ***************************************************************************/

class TCPSocket : public Socket
{
	
	public:
		
	//	Constructors
	//=-=-=-=-=-=-=-=-=
		~TCPSocket();
		TCPSocket(); //default
		TCPSocket(int sockfd); //copy constructor
		TCPSocket(std::string addr, int port); //create and bind

	//	TODO State queries
	//=-=-=-=-=-=-=-=-=
		//bool isConnected();

	//	Socket Options
	//=-=-=-=-=-=-=-=-=
		/*
 		 * setTimeout: sets the connection timeout for the socket
 		 *	params: timeout = the timeout period in seconds
		 *	default timeout = 10 secs
 		 */
		void setTimeout(int timeout);

	//	Connections
	//=-=-=-=-=-=-=-=-=
		/*
		 * connect: establish a connection to a peer at addr on the given port
		 *
		 */

		void connect(const char *addr, int port);
		/*
 		 * listen: puts this socket into a listening state, in which requests are backlogged for eventual aceptance.
 		 */

		void listen(int backlog_size=5);

		/*
		 * close
		 */
		void close();

		/*
		 * accept: connect to the first request in the socket's backlog.
		 *		Returns: a new TCPSocket to handle the connection
		 *
		 *	TODO see man accept for info on detecting incoming connection requests
		 * 	with select, poll or SIGIO
		 */
		TCPSocket* accept();



	//	Send & Receive
	//=-=-=-=-=-=-=-=-=
		/*
		 * send: send will accept either a byte array or an istream. 
		 *	A TCP socket must have an established connction to send.
		 */
		void send(char *input);
		void send(std::istream& input);


	private:
		bool closed;
		//TODO -- maintain information about this socket's connection status
		//should include peer address, port, role(client/server), ??
		//char *connectionInfo;

//		Inherited from Socket
//=-=-==-=-=-=-=-=-=-=-=-=-=-=-=
		//int getSocket();
		//SocketType getSocketType();
		//std::string getBoundAddr();
		//int getBoundPort();
		/*
		 * bind: associates the socket with an IP address and port number
		 *	params
		 *		int port (required): the port number
		 *		string addr (optional): the IP address as a string. default is to 
		 *			associate with any valid address on the system
		 *
		 * NOTE: bind is mostly optional in tcp sockets. Mainly useful if you 
		 * 	want to associate with some other ip address?
		 */
		//void bind(int port, std::string addr="0.0.0.0");

};
#endif
