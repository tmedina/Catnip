EXTRA_CFLAGS =  -g 
catnip: catnip.o
	$(CXX) -L/home/tmedina/Projects/Catnip/libcatnip catnip.o -o catnip -lcatnip

catnip.o: catnip.cpp
	$(CXX) -I/usr/include -I/home/tmedina/Projects/Catnip/libcatnip -c catnip.cpp $(EXTRA_CFLAGS)

clean:
	rm *.o catnip

