# Makefile
# Created by Simon Mellberg (hed16smg)
CC = cc

CFLAGS  += 	-std=gnu11 -Wall -Wextra -Werror -Wmissing-declarations \
			-Wmissing-prototypes -Werror-implicit-function-declaration \
			-Wreturn-type -Wparentheses -Wunused -Wold-style-definition \
			-Wundef -Wshadow -Wstrict-prototypes -Wswitch-default -Wunreachable-code\
			-pedantic

INCLUDE = -I ./list.c:./list.h:./mpasswdsort.h 

OBJECTS = mpasswdsort.o list.o 

mpasswdsort: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o mpasswdsort 

mpasswdsort.o: mpasswdsort.c mpasswdsort.h list.h
	$(CC) $(CFLAGS) -c mpasswdsort.c
	
list.o: list.c list.h	
	$(CC) $(CFLAGS) -c list.c
	
all: mpasswdsort

clean :
	-rm $(OBJECTS) mpasswdsort
	