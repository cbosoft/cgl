CXX = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -g
LINK = 
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
	$(CXX) $(CFLAGS) -o $@ src/main.c -L`pwd` -lcgl

clean:
	rm -rf *.o
