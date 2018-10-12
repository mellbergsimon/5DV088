# Makefile
# Created by Simon Mellberg (hed16smg)
CC = cc

CFLAGS  += 	-std=gnu11 -Wall -Wextra -Werror -Wmissing-declarations \
			-Wmissing-prototypes -Werror-implicit-function-declaration \
			-Wreturn-type -Wparentheses -Wunused -Wold-style-definition \
			-Wundef -Wshadow -Wstrict-prototypes -Wswitch-default -Wunreachable-code\
			-pedantic

OBJECTS = execute.o parser.o sighant.o mish.o

mish: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o mish 
	
mish.o: mish.c mish.h
	$(CC) $(CFLAGS) -c -g mish.c
	
parser.o: parser.c parser.h
	$(CC) $(CFLAGS) -c -g parser.c
	
sighant.o: sighant.c sighant.h
	$(CC) $(CFLAGS) -c -g sighant.c
	
execute.o: execute.c execute.h
	$(CC) $(CFLAGS) -c -g execute.c
	
all: mish

valgrind: mish
	valgrind --leak-check=full --show-reachable=yes --track-origins=yes ./mish

clean :
	-rm $(OBJECTS)
	
