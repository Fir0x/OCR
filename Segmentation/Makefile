CC=gcc
CFLAGS= -Wall -Wextra -Werror -std=c99 -O3 -o
LDFLAGS=
LDLIBS=
EXEC= main
SRC= ../Misc/matrix.c ../Misc/random.c ../Misc/list.c bloc.c
OBJ = ${SRC:.c=.o}

all: net

net: ${OBJ}
	gcc ${CFLAGS} ${EXEC} $^

%.o: %.c
	gcc ${CFLAGS} $@ -c $<

clean:
	rm ${EXEC}
	rm -f *.o
	rm -f ../Misc/*.d
	
