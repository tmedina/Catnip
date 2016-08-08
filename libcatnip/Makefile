libcatnip: Socket.o TCPSocket.o UDPSocket.o MulticastSocket.o 
	$(CXX) -shared -L/usr/lib/ -Wl,-soname,libcatnip.so -o libcatnip.so.1.0.3 Socket.o TCPSocket.o UDPSocket.o MulticastSocket.o -lc -lgcc
	ln -s libcatnip.so.1.0.3 libcatnip.so

	#$(CXX) -shared -Wl,-install_name,libcatnip.so -o libcatnip.so.1.0.2 Socket.o TCPSocket.o UDPSocket.o MulticastSocket.o /usr/lib/libgcc_s.10.4.dylib /usr/lib/libc.dylib

Socket.o: Socket.cpp TCPSocket.cpp UDPSocket.cpp MulticastSocket.cpp 
	$(CXX) -g -I/usr/include -c Socket.cpp TCPSocket.cpp UDPSocket.cpp MulticastSocket.cpp -fpermissive -fPIC

clean:
	rm *.o libcatnip.*

#Mac OSX: change soname to install_name
	#$(CXX) -shared -Wl,-install_name,libcatnip.so -o libcatnip.so.1.0.2 Socket.o TCPSocket.o UDPSocket.o MulticastSocket.o /usr/lib/libgcc_s.10.4.dylib /usr/lib/libc.dylib
