all: cat

lib:
	$(MAKE) -C libcatnip

cat: lib
	$(MAKE) -C catnip

clean:
	$(MAKE) clean -C libcatnip
	$(MAKE) clean -C catnip
