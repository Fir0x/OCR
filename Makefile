CC=gcc
CFLAGS= `pkg-config --cflags sdl gtk+-3.0` -Wall -Wextra -std=c99 -O3\
		-o
LDFLAGS=
LDLIBS= `pkg-config --libs sdl gtk+-3.0` -lSDL_image -lm
EXEC= ocr
SRC= Misc/matrix.c Misc/random.c Segmentation/bloc.c Misc/list.c\
	 Binarize/sauvola.c Binarize/pixel_operations.c NeuralNet/layer.c\
	 NeuralNet/train.c NeuralNet/neural_net.c NeuralNet/save_net.c\
	 Graphical_Interface/main.c

OBJ=${SRC:.c=.o}

all: net

net: ${OBJ}
	gcc ${CFLAGS} ${EXEC} $^ ${LDLIBS}
	cp Graphical_Interface/main_window.glade main_window.glade
	make clean

%.o: %.c
	gcc ${CFLAGS} $@ -c $<

.PHONY: clean mrproper

clean:
	find . -name \*.o -type f -delete
	find . -name \*.d -type f -delete

mrproper: clean
	${RM} ${EXEC}
	${RM} main_window.glade
	${RM} vgcore.*
	${RM} output.txt
