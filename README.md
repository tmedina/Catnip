# Catnip
Catnip aims to be an easy-to-use C++ sockets library for sending and receiving tcp, udp and multicast messsages.

It consists of `libcatnip`, the backend C++ API, and `catnip` a shell utility front-end with an interface inspired by the venerable netcat utility.

Catnip currently supports Linux on x86 architectures.

## Catnip shell utility
### TCP Sockets
##### Pipe a message to a tcp socket:
`echo "Hello TCP" | tcp -a 192.168.1.1 -p 9999`

##### Open a TCP socket and listen for incoming messages:
`tcp -l -a 192.168.1.1 -p 9999`

##### File transfer
`cat file.pdf | tcp -a 192.168.1.1 -p 9999`

### Multicast Sockets
##### Pipe a message to a multicast group:
`echo "Hello Multicast" | multi -a 224.1.1.2 -p 8888`

##### Join a multicast group and listen for messages:
`multi -a 224.1.1.2 -p 8888`

### UDP Sockets
##### Pipe a message to a UDP socket:
`echo "Hello UDP" | udp -a 192.168.1.1 -p 7777`

##### Open a UDP socket and listen for messages:
`udp -a 192.168.1.1 -p 7777`

## libcatnip API
### TCP Sockets
##### Receive TCP messages
```c++
char *h;
TCPSocket tcp;
tcp.bind(9997);
tcp.listen();
TCPSocket *newtcp = tcp.accept();
newtcp->receive(&h);
printf("%s\n", h);
free(h);
delete(newtcp);
```

##### Send a TCP message
```c++
TCPSocket tcp2sender;
tcp2sender.connect("127.0.0.1", 9997);
tcp2sender.send("Hello TCP!\n");
```
##### Send a File over TCP
```c++
TCPSocket tcp;
tcp.connect("192.168.3.56", 9998);
::ifstream sendFile;
sendFile.open("Test.pdf", ios::in | ios::binary);
tcp.send(sendFile);
```
##### Receive a file over TCP
```c++
char *h;
int hSize;
TCPSocket tcp;
tcp.bind(9999);
tcp.listen();
TCPSocket *tcp_receiver = tcp.accept();
::ofstream qFile("Test.pdf", ios::out | ios::binary);
hSize = tcp_receiver->receive(qFile);
free(h);
delete(tcp_receiver);
```
### UDP Sockets
##### Send a message over UDP
```c++
UDPSocket udp1;
udp1.send("Hello UDP!\n", "127.0.0.1", 9997);
```
##### Receive a message over UDP
```c++
char * h;
UDPSocket udp;
udp.bind(9997);
udp.receive(&h);
free(h);
```
### Multicast Sockets
##### Send a message over multicast
```c++
string maddr = "224.0.0.1";
int mport = 8564;
MulticastSocket mcast_sender;
mcast_sender.send("Hello Multicast!", maddr.c_str(), mport);
```

##### Receive a multicast message
```c++
char *h;
string maddr = "224.0.0.1";
int mport = 8564;
MulticastSocket mcast;
mcast.subscribe(maddr.c_str());	
mcast.bind(mport);
mcast.receive(&h);
printf("%s\n", h);
free(h);
```
