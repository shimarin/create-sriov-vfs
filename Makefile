PREFIX ?= /usr/local

.PHONY: all clean install

all: create-sriov-vfs

create-sriov-vfs: create-sriov-vfs.cpp
	g++ -std=c++23 -o $@ $^

clean:
	rm -f create-sriov-vfs

install: create-sriov-vfs
	install -d $(DESTDIR)$(PREFIX)/bin
	install -m 755 create-sriov-vfs $(DESTDIR)$(PREFIX)/bin
