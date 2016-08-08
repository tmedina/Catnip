#include "UDPSocket.h"
#include <stdio.h>
#include <unistd.h>

/*
 * TODO:
 *		Support Broadcast messages - I think you can do this by just sending to the broadcast address?
 */
using namespace std;

//=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=--
//				UDP Socket Implementation
//=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=--
UDPSocket::UDPSocket()
:Socket(AF_INET, SOCK_DGRAM)
{
	setSocketType(Socket::UDP);

}

UDPSocket::UDPSocket(int sockfd)
:Socket(sockfd)
{
	setSocketType(Socket::UDP);

}

UDPSocket::~UDPSocket()
{
	close(getSocket());
}

void UDPSocket::send(const char *msg, const char *addr, int port)
{
	Socket::sendmsg((char *)msg, addr, port, -1);
}

void UDPSocket::send(char *msg, const char *addr, int port)
{
	Socket::sendmsg(msg, addr, port, -1);
}

void UDPSocket::send(::istream& in, const char *addr, int port)
{
	Socket::sendstream(in, addr, port);
}

/*
 * receive:
 */
 // Inherited from Socket

