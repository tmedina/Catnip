#include "TCPSocket.h"
#include <stdio.h>
#include <unistd.h>
#include <deque>


using namespace std;

//=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=--
//				TCP Socket Implementation
//=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=--

//TODO possibly support AF_LOCAL/AF_UNIX sockets
TCPSocket::TCPSocket()
:Socket(AF_INET, SOCK_STREAM)
{
	setSocketType(Socket::TCP);

}

TCPSocket::TCPSocket(int sockfd)
:Socket(sockfd)
{
	setSocketType(Socket::TCP);

}

TCPSocket::~TCPSocket()
{
//	printf("Destroying socket %d\n", getSocket());
	if(! closed )
	{
		close();
	}

}

void TCPSocket::close()
{
	this->closed = true;
	::shutdown(getSocket(), 2);
	::close(getSocket());
}

void TCPSocket::send(::istream &in)
{
	Socket::sendstream(in, NULL, 0);
}

void TCPSocket::send(char *in)
{
	Socket::sendmsg(in, NULL, 0, -1);
}

// connect to a peer at addr on the given port
void TCPSocket::connect(const char *addr, int port)
{
	//printf("Starting connect: %s, %d\n",addr, port);
	struct sockaddr_in serverAddr; 
	memset((char *)&serverAddr, 0, sizeof(serverAddr)); 


	//TODO: enable AF_LOCAL
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr(addr); 
	serverAddr.sin_port = htons( port );

	if ( (::connect(getSocket(), (struct sockaddr *) &serverAddr, sizeof(serverAddr))) < 0) 
	{
		char buffer[ 256 ];
		char * errorMessage = strerror_r( errno, buffer, 256 );
		throw(errorMessage);
	}
	this->closed = false;
}

/*
 * setTimeout: sets the connection timeout for the socket
 *	params: timeout = the timeout period in seconds
 */
void TCPSocket::setTimeout(int timeout)
{
	struct timeval tv;
	tv.tv_sec = timeout;
	setsockopt(getSocket(), SOL_SOCKET, SO_RCVTIMEO,(struct timeval *)&tv,sizeof(struct timeval));
}

/*
 * listen: puts this socket into a listening state, in which requests are backlogged for eventual aceptance.
 */
void TCPSocket::listen(int backlog_size)
{
	int r;
	if( (r = ::listen(getSocket(), backlog_size)) < 0)
	{
		char buffer[ 256 ];
		char * errorMessage = strerror_r( errno, buffer, 256 );
		throw(errorMessage);
	}
	
}

/*
 * accept: connect to the first request in the socket's backlog.
 *		Returns: a new TCPSocket to handle the connection
 *
 *	see man accept for info on detecting incoming connection requests
 * 	with select, poll or SIGIO
 */

TCPSocket* TCPSocket::accept()
{
	int newfd = 0;
	struct sockaddr *peer;
	socklen_t addrlen = 0;
	while( (newfd = ::accept4(getSocket(), peer, &addrlen, SOCK_NONBLOCK)) <= 0)
	{
		int err = errno;
   	char buffer[ 256 ];
		char* errorMessage;
		switch(newfd)
		{
			case -1:
				if(err == EAGAIN || err == EWOULDBLOCK) continue;
				else 
				{
					errorMessage = strerror_r( err, buffer, 256 );
					throw(errorMessage);
				}
			case 0:
				errorMessage = strerror_r( err, buffer, 256 );
				throw(errorMessage);
			default:
				char * errorMessage = strerror_r( err, buffer, 256 );
				throw(errorMessage);
		}
	} 

	return new TCPSocket(newfd);
		
}

