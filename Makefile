CXX = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -g
LINK = 
SO = libcgl.so
OBJ = obj/cgl.o
HDR = src/cgl.h

.PHONY: shared

shared: $(SO)

%.so: $(OBJ)
	$(CXX) -shared -o $@ $(CFLAGS) $(OBJ) $(LINK)

obj/%.o: src/%.c
	mkdir -p `dirname $@`
	$(CXX) $(CFLAGS) -c -o $@ -fPIC $<

clean:
	rm -rf *.o
