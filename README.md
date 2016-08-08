# Catnip
An easygoing sockets library with a netcat-inspired application frontend

Some example uses:
Pipe a message to a tcp socket:
`echo "Hello Catnip" | tcp -a 192.168.1.1 -p 9999`
Open a TCP socket and listen for incoming messages:
`tcp -l -a 192.168.1.1 -p 9999`

Pipe a message to a multicast group:
`echo "Hello Multicast" | multi -a 224.1.1.2 -p 8888`
Join a multicast group and listen for messages:
`multi -a 224.1.1.2 -p 8888`

Pipe a message to a UDP socket:
`echo "Hello UDP" | udp -a 192.168.1.1 -p 7777`
Open a UDP socket and listen for messages:
`udp -a 192.168.1.1 -p 7777`
