CC=gcc
CFLAGS= -Wall -Wextra -std=c99 -O3 -o
CPPFLAGS=`pkg-config --cflags sdl` -MMD
LDFLAGS=
LDLIBS= `pkg-config --libs sdl` -lSDL_image -lm
EXEC= main
SRC= matrix.o random.o bloc.o list.o sauvola.o pixel_operations.o layer.c train.c\
	 neural_net.c main.c save_net.c

all: net

net: ${SRC}
	gcc ${CPPFLAGS} ${CFLAGS} ${EXEC} ${SRC} ${LDLIBS}
	make clean

matrix.o: ../Misc/matrix.c
	gcc ${CFLAGS} $@ -c $< 

random.o: ../Misc/random.c
	gcc ${CFLAGS} $@ -c $<

otsu.o: ../Binarize/otsu.c
	gcc ${CFLAGS} $@ -c $<

pixel_operations.o: ../Binarize/pixel_operations.c
	gcc ${CFLAGS} $@ -c $<

bloc.o: ../Segmentation/bloc.c
	gcc ${CFLAGS} $@ -c $<

list.o: ../Misc/list.c
	gcc ${CFLAGS} $@ -c $<

binarize.o: ../Binarize/binarize.c
	gcc ${CFLAGS} $@ -c $<

sauvola.o: ../Binarize/sauvola.c
	gcc ${CFLAGS} $@ -c $<

.PHONY: clean mrproper

clean:
	${RM} -f *.o
	${RM} -f ../Misc/*.d
	${RM} -f *.d

mrproper: clean
	${RM} ${EXEC}
