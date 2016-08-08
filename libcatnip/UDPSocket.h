#ifndef UDPSOCKET_H
#define UDPSOCKET_H

#include "Socket.h"

/***************************************************************************
 *
 *	UDPSocket 
 * ------------------
 *
 *	Author: Terrance Medina
 *
 *	class UDPSocket: 
 *
 *	provides a UDP socket for use in network applications.
 *
 *
 ***************************************************************************/


class UDPSocket : public Socket
{
	public:
		~UDPSocket();
		UDPSocket();
		UDPSocket(int sockfd);
		/*
		 * send
		 */
		void send(char *msg, const char* addr, int port);
		void send(const char *msg, const char* addr, int port);
		void send(std::istream& input, const char* addr, int port);

	private:

};
#endif
