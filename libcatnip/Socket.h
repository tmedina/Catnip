#ifndef SOCKET_H
#define SOCKET_H


#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <string>
#include <ostream>
#include <istream>


/***************************************************************************
 *
 *	Socket Library
 * ------------------
 *
 *	Author: Terrance Medina
 *
 *	class Socket: 
 *
 *	provides a base class to be extended by specific socket implementations.
 *	Send and Receive functions are protected and should be called by derived
 *	socket implementations, with appropriate parameter values.
 *
 * Refs:
 *	man 3 inet
 *	man 7 ip
 *  man socket
 *
 ***************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif
//TODO make this abstract
class Socket
{
	
	public:
		//TODO use these (and more) as part of a robust exception handling mechanism!
		enum SocketError {ECONNECTFAIL, ESNDFAIL, ECREATFAIL};

		/*
		 *	SocketType: Tells a socket what kind it is
		 *		TCP: Tcp socket
		 *		UDP: Udp socket
		 *		MULTI: Multicast socket
		 */
		enum SocketType {TCP, UDP, MULTI};

	//=-=-==-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	//					CONSTRUCTORS
	//=-=-==-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		~Socket();
		Socket();//default
		Socket(int family, int type);
		//Build a Socket object around an existing socket descriptor
		Socket(int sockfd);

	//=-=-==-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	//					GETTERS & SETTERS
	//=-=-==-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		/*
		 *	getSocket():
		 * 		Return the unix socket descriptor for direct manipulation
		 */
		int getSocket();

		/*
		 *	getSocketType():
		 *		Return the enumerated type of this socket
		 */
		SocketType getSocketType();

		/*
		 *	setSocketType():
		 *		Set the type of this socket
		 */
		void setSocketType(SocketType);

		/*
		 *	getBoundAddr():
		 *		Return the address to which this socket is bound, or NULL if it has not been bound yet
		 */
		std::string getBoundAddr();

		/*
		 *	getBoundPort():
		 *		Return the port number to which this socket is bound, or NULL if it has not been bound yet
		 */
		int getBoundPort();

		/*
		 *	getFamily():
		 *		Return the family for which this socket was created.
		 *		Currently only supports AF_INET
		 */
		int getFamily();

		/*
		 *	setFamily():
		 *		Set the family for which this socket was created.
		 *		Currently only supports AF_INET
		 */
		void setFamily(int family);

		//TODO
		void setBlocking();

		//TODO make send a virtual function
		//virtual void send();

		int receive(char **response, int recv_size = -1);
		int receive(std::ostream& in, int recv_size = -1);

		/*
		 * bind: associates the socket with an IP address and port number
		 *	params
		 *		int port (required): the port number
		 *		string addr (optional): the IP address as a string. default is to 
		 *			associate with any valid address on the system
		 */
		void bind(int port, std::string addr="0.0.0.0");


	//=-=-==-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	//					SEND & RECEIVE
	//=-=-==-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	protected:
		/*
		 * send
		 */
		void sendstream(std::istream& input, const char* addr = NULL, int port = 0);
		void sendmsg(char *msg, const char* addr = NULL, int port = 0, int size = -1);
		void sendmsg(const char *msg, const char* addr = NULL, int port = 0, int size = -1);

		/*
		 * receive
		 * Use cases:
		 *	1. read everything from a socket until it is closed by the sender (as in file/message transfer)
		 * 		WARNING: will block until properly closed at other end // set timeout?
		 *		TODO: This doesn't make sense for UDP (There is no connection to close), read until empty
		 *	2. read N-bytes from the socket (as in a media/GPS data stream)
		 *	3. TODO: make a best effort at reading N-bytes, but return whatever you've got
		 *	4. Read until the socket is empty (complete or not)
		 *  5. TODO: blocking and non-blocking variants?
		 *
		 *	IMPORTANT: You are responsible for de-allocating memory pointed to by the *response handle.
		 */
		int receive_all(char **response);
		int receive_part(char **response, int size);


	private:
		//the socket file descriptor
		int sock;

		//the enumerated type of this socket
		SocketType type;

		//the ip address to which this socket has been bound
		std::string boundAddr;

		//the port to which this has been bound
		int boundPort;

		//the family type of this socket
		int family;

		//TODO some data structures for keeping operating statistics

};
#ifdef __cplusplus
}
#endif
#endif
