CXX = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -g $(shell pkg-config --cflags gtk+-3.0)
LINK   = $(shell pkg-config --libs gtk+-3.0) -lm
SO = libcgl.so
OBJ = obj/cgl.o
HDR = src/cgl.h

.PHONY: test_and_run

test_and_run: test
	LD_LIBRARY_PATH=`pwd` ./test

.PHONY: shared

shared: $(SO)

%.so: $(OBJ)
	$(CXX) -shared -o $@ $(CFLAGS) $(OBJ) $(LINK)

obj/%.o: src/%.c
	mkdir -p `dirname $@`
	$(CXX) $(CFLAGS) -c -o $@ -fPIC $<

test: shared
	$(CXX) $(CFLAGS) -o $@ src/main.c -L`pwd` -lcgl $(LINK)

clean:
	rm -rf obj $(SO) 
