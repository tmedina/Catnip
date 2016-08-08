#include "MulticastSocket.h"
#include <stdio.h>
#include <unistd.h>
#include <deque>


using namespace std;

//=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=--
//				Multicast Socket Implementation
//=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=--
MulticastSocket::MulticastSocket()
:UDPSocket()
{
	setSocketType(Socket::MULTI);

	int reuse=1;
	int mcast_ttl=16;
 
    if (setsockopt(getSocket(), SOL_SOCKET, SO_REUSEADDR,
                   (char *)&reuse, sizeof(reuse)) < 0) {
			char buffer[ 256 ];
			char * errorMessage = strerror_r( errno, buffer, 256 );
			throw(errorMessage);
    }
    if ((setsockopt(getSocket(), IPPROTO_IP, IP_MULTICAST_TTL, &mcast_ttl, sizeof(mcast_ttl))) < 0){
			char buffer[ 256 ];
			char * errorMessage = strerror_r( errno, buffer, 256 );
			throw(errorMessage);
	} 

}

MulticastSocket::~MulticastSocket()
{
	close(getSocket());
}

void MulticastSocket::subscribe(string addr)
{
	//printf("subscribing to %s !!!\n", addr.c_str());
	struct ip_mreq mreq;
	unsigned char mcast_ttl=16;

	//set multicast address
	mreq.imr_multiaddr.s_addr = inet_addr(addr.c_str());

	//set local address
	string b_addr = getBoundAddr();
	if(b_addr == "")
		mreq.imr_interface.s_addr = INADDR_ANY;
	else
		//mreq.imr_interface.s_addr = htonl(inet_addr(b_addr.c_str()));
		mreq.imr_interface.s_addr = INADDR_ANY;

	//if (setsockopt(getSocket(), IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(struct ip_mreq)) == -1) {
	if (setsockopt(getSocket(), IPPROTO_IP, IP_MULTICAST_TTL, &mcast_ttl, sizeof(mcast_ttl)) == -1) {
//		printf("Killed it\n");
		char buffer[ 256 ];
		char * errorMessage = strerror_r( errno, buffer, 256 );
		throw(errorMessage);
  	}

	//TODO: add addr to list of subscriptions
	
	return;
}

void MulticastSocket::unsubscribe(char *addr)
{
	printf("unsubscribing from %s !!!\n", addr);
	return;
}

/*
//Wrapper for BSD sendto syscall
// TODO: add bytes to throughput counter for stats collection
void MulticastSocket::send(char *msg, const char* addr, int port)
{
	struct sockaddr_in destaddr; // destination address
	memset((char *)&destaddr, 0, sizeof(destaddr)); 

	destaddr.sin_addr.s_addr = htonl(inet_addr(addr)); 
	destaddr.sin_family = this->getFamily();
	destaddr.sin_port = htons(port);

	if(msg == NULL) return;

	int msglen = strlen(msg);
	//printf("Sending message: %s\n", msg);
	//printf("Sending size %d\n", msglen);

	//MSG_NOSIGNAL - allows send to return if unconnected
	if ( ::sendto(getSocket(), msg, msglen, MSG_NOSIGNAL, (struct sockaddr*)&destaddr, sizeof(destaddr)) != msglen ) {
		throw(errno);
	}
}
*/
