#include "Socket.h"
#include <stdio.h>
#include <unistd.h>
#include <deque>
#include <list>



using namespace std;

//=-=-==-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//		CONSTRUCTORS
//=-=-==-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
Socket::Socket()
{
	//do nothing?
}

Socket::Socket(int family, int type)
{
	//TODO: support AF_UNIX?
	setFamily(family);

	//create a socket for the base class
	//TODO: check for option flags?
	if( (this->sock = socket(family, type, 0)) < 0 )
	{
		throw(errno);
	}

}

//Create a Socket object from a given file descriptor
Socket::Socket(int sockfd)
{
	//TODO: support AF_UNIX?
	//TODO get family from sockfd
	setFamily(AF_INET);

	this->sock = sockfd;
}

Socket::~Socket()
{
	close(this->sock);

}

//Return the unix socket descriptor
int Socket::getSocket()
{
	return this->sock;
}

//Return the type of this socket
Socket::SocketType Socket::getSocketType()
{
	return this->type;
}

//Set the type of this socket
void Socket::setSocketType(Socket::SocketType t)
{
	this->type = t;
}

string Socket::getBoundAddr()
{
	return this->boundAddr;
}

int Socket::getBoundPort()
{
	return this->boundPort;
}

int Socket::getFamily()
{
	return this->family;
}

void Socket::setFamily(int family)
{
	this->family = family;
}


void Socket::sendstream(::istream& input, const char* addr, int port)
{
	int tmpsz = 1024;
	char tmp[tmpsz];
	while(input.good())
	{
		input.read(&tmp[0], tmpsz);
		int count = input.gcount();
		//printf("Got %d bytes from file\n", count);
		sendmsg(&tmp[0], addr, port, count);
		if(input.eof()) return;
	}
	if(input.fail()) throw(errno);//TODO get appropriate exception
	
}

//Wrapper for BSD sendto syscall
// TODO: add bytes to throughput counter for stats collection
void Socket::sendmsg(char *msg, const char* addr, int port, int size)
{
	if(msg == NULL) return;
	int msglen = size == -1 ? strlen(msg) : size;
	//printf("Sending message: %s\n", msg);
	//printf("Sending size %d\n", msglen);

	struct sockaddr_in dest;
	if(addr != NULL && port >= 0)
	{
		memset((char *)&dest, 0, sizeof(dest)); 
		dest.sin_addr.s_addr = inet_addr(addr); 
		dest.sin_family = this->getFamily();
		dest.sin_port = htons(port);
	}

	socklen_t dest_len = sizeof(dest);

	//MSG_NOSIGNAL - allows send to return if unconnected
	if ( ::sendto(getSocket(), msg, msglen, 0, (struct sockaddr*)&dest, dest_len) != msglen ) {
	//if ( ::sendto(getSocket(), msg, msglen, MSG_NOSIGNAL, (struct sockaddr*)&dest, dest_len) != msglen ) {
		throw(errno);
	}
}

void Socket::sendmsg(const char *msg, const char* addr, int port, int size)
{
	if(msg == NULL) return;
	int msglen = size == -1 ? strlen(msg) : size;
	//printf("Sending message: %s\n", msg);
	//printf("Sending size %d\n", msglen);

	struct sockaddr_in dest;
	if(addr != NULL && port >= 0)
	{
		memset((char *)&dest, 0, sizeof(dest)); 
		dest.sin_addr.s_addr = inet_addr(addr); 
		dest.sin_family = this->getFamily();
		dest.sin_port = htons(port);
	}

	socklen_t dest_len = sizeof(dest);

	//MSG_NOSIGNAL - allows send to return if unconnected
	if ( ::sendto(getSocket(), msg, msglen, 0, (struct sockaddr*)&dest, dest_len) != msglen ) {
	//if ( ::sendto(getSocket(), msg, msglen, MSG_NOSIGNAL, (struct sockaddr*)&dest, dest_len) != msglen ) {
		throw(errno);
	}
}

int Socket::receive(::ostream& in, int recv_size)
{
	char *handle = 0;
	if(recv_size == -1) // read EVERYTHING from the socket. This could get BIG.
	{
		int h_size = receive_all(&handle);
		//printf("Received %d bytes\n", h_size);
		in.write(handle, h_size);
		in.flush();
		if(handle != 0) free(handle);
		return h_size;
		
	}
	else // read at most recv_size bytes from the socket
	{
		int r = receive_part(&handle, recv_size);
		//printf("Received %d bytes\n", r);
		in.write(handle, r);
		in.flush();
		if(handle != 0) free(handle);
		return r;
		
	}
	return -1;
}

//Use cases:
//	1. read everything from a socket until it is closed by the sender (as in file/message transfer)
	// WARNING: will block until properly closed at other end, //or timeout?
	// WARNING: this will overflow your heap if you are not careful!
//	2. read an N-bytes chunk from the socket (as in a media/GPS data stream)
//	3. TODO: make a best effort at reading N-bytes, but return whatever you've got
//  4. TODO: blocking and non-blocking variants?

//Returns:
//	pointer to memory where the message is stored (User must free!)
//	size of message
int Socket::receive(char **response, int recv_size)
{
	if(recv_size == -1) //Read from socket until closed at the other end
	{
		int r = receive_all(response);
		return r;
	}
	else
	{
		int r;
		try{
			r = receive_part(response, recv_size);
		}
		catch(int e)
		{
			printf("Caught error from receive_part: %s\n", strerror(e));
			(*response)[r] = '\0';
		}
		return r;
	}
}

//Utility struct for mapping heap data locations in receive_all
struct mem_chunk
{
	char *pointer;
	int size;
};

int Socket::receive_all(char **response)
{
	//Read from socket until closed at the other end
	// WARNING: will block until properly closed at other end // set timeout?

	int totalbytes = 0;

	list<mem_chunk> data; //maintains a list of pointers to heap memory and sizes, for tracking data read from the socket
	char *d; //reusable pointer to heap memory
	int r;//size of response

	int tmpsize = 1024;
	char tmp[tmpsize];//temporary holder for socket response

	bool started = false;//used for time benchmarking


	//printf("Starting to receive_all...\n");
	//Last 2 params are ignored on a TCP socket
	while ( (r = ::recv(sock, &tmp, tmpsize, MSG_DONTWAIT)) > -2 )
	{ 
		int err = errno;
		switch(r)
		{
			case -1:
				if(err == EAGAIN || err == EWOULDBLOCK)
				{
					//TODO: this is a gross hack, because the base class should not care which
					//subclass is running this, but I also don't want to duplicate
					//this entire function b/c of one line difference
					if(getSocketType() == Socket::TCP)
					{
						break; //nothing to read now, but try again
					}
					else if(getSocketType() == Socket::UDP || getSocketType() == Socket::MULTI)
					{
						//Once we have started receiving data, we should go ahead and return on a -1
						if(!started) break;
						*response = (char *)malloc(totalbytes+1); //+1 for '\0' padding
						char *r_it = *response;
		
						for(::list<mem_chunk>::iterator it = data.begin(); it != data.end(); it++)
						{
							memcpy(r_it, it->pointer, it->size);
							r_it += it->size;
							//free heap memory
							free(it->pointer);
						}
						
						*r_it = '\0'; //EOL padding
						//printf("Returning buffer of size %d\n", strlen(*response));
			
						return totalbytes;

					}
					else //default
					{
						break;
					}
				}
				else
				{
					throw(err);//there really is a problem, abort
				}
				break;
			case 0: // socket has been properly closed, let's go home
			{
				//printf("recv returned 0. Socket probably closed.\n");
				*response = (char *)malloc(totalbytes+1);
				char *r_it = *response;

				for(::list<mem_chunk>::iterator it = data.begin(); it != data.end(); it++)
				{
					memcpy(r_it, it->pointer, it->size);
					r_it+=it->size;
					//free heap memory
					free(it->pointer);
				}
				*r_it = '\0'; //EOL padding

				//TODO: optional time benchmarking
				return totalbytes;
			}
			default: //store data in the heap
			{
				if (r > 0)
				{
					//optional time benchmarking
					if(!started)
					{
						started = true;
						//system("date");
					}
					//copy received data into heap location
					d = (char*)malloc(r);
					memcpy(d, &tmp, r);


					//keep pointer to heap location in data list
					struct mem_chunk m;// = (mem_chunk*)malloc(sizeof(mem_chunk));
					m.pointer = d;
					m.size = r;
					data.push_back(m);

					totalbytes += r;
					//printf("Total received = %d bytes\n",totalbytes);
					
				}
				break;
			}
		}
  	if(getSocketType() != Socket::TCP)
			usleep(1000); //these are non-blocking for UDP, so we need to sleep
	}
}	

int Socket::receive_part(char **response, int recv_size)
{
	int bytes = 0;
	int received = 0;
	char *buffer = (char *)malloc(recv_size);
	char *bufptr = buffer;

	while (received < recv_size) {

		//printf("Starting to receive ...\n");
		if ((bytes = ::recv(sock, bufptr, recv_size - received, 0)) < 0) {
			int err = errno;
			//printf("Received %d\n", bytes);
			if(getSocketType() == Socket::TCP)
			{
				if(err == EAGAIN || err == EWOULDBLOCK) continue;
				else throw(err);
			}
			else if(getSocketType() == Socket::UDP || getSocketType() == Socket::MULTI)
			{
				//Socket is probably closed. Return the partial result.
				*bufptr = '\0';
				*response = buffer;
				return received+1;
			}
		}
		else if(bytes == 0)
		{
			//printf("Received %d\n", bytes);
			//Socket is closed. Return the partial result.
			*bufptr = '\0';
			*response = buffer;
			return received+1;
		}
		
		bufptr += bytes;
		received += bytes;
		//printf("bytes %d .... || %s\n", received, buffer);
	}
	*response = buffer;
	return received;
}



/*
 * bind: associates the socket with an IP address and port number
 *	params
 *		int port (required): the port number
 *		string addr (optional): the IP address as a string. default is to 
 *			associate with any valid address on the system (0.0.0.0)
 */
void Socket::bind(int port, string addr)
{
	struct sockaddr_in myaddr; // our address
	memset((char *)&myaddr, 0, sizeof(myaddr)); 

	myaddr.sin_addr.s_addr = inet_addr(addr.c_str()); 
	myaddr.sin_family = this->getFamily();
	myaddr.sin_port = htons(port);

	if (::bind(getSocket(), (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) 
	{ 
		throw(errno);
	}

	//Once the bind has completed successfully, assign our private variables
	this->boundAddr =  ntohl(inet_ntoa(myaddr.sin_addr));
	this->boundPort = ntohs(myaddr.sin_port);
}

