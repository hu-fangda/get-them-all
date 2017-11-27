CC  = gcc
CXX = g++

INCLUDES =
CFLAGS   = -g -Wall $(INCLUDES)
CXXFLAGS = -g -Wall $(INCLUDES)

LDFLAGS = -g
LDLIBS =

.PHONY: default
http-client: http-client.o
http-client.o: http-client.c

.PHONY: clean
clean:
	rm -f *.o *~ a.out core http-client

.PHONY: all
all: clean default

