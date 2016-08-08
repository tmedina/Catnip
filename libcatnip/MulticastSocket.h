#ifndef MULTICASTSOCKET_H
#define MULTICASTSOCKET_H

#include "Socket.h"
#include "UDPSocket.h"


/***************************************************************************
 *
 *	MulticastSocket 
 * ------------------
 *
 *	Author: Terrance Medina
 *			tmedina@utility.com
 *
 *	Copyright 2014 Utility Associates
 *
 *
 *	class MulticastSocket: 
 *
 *	provides a Multicast socket for use in network applications.
 *
 *
 ***************************************************************************/

class MulticastSocket : public UDPSocket
{
	public:
		~MulticastSocket();
		MulticastSocket();
		MulticastSocket(int);

		void subscribe(std::string addr);

		//TODO
		void unsubscribe(char *addr);
		//TODO
		//getSubscriptions()
		//void send(char *msg, const char* addr, int port);

		//int receive(char **handle, std::string from_addr, int port, int recv_size=-1);

	private:

};
#endif
