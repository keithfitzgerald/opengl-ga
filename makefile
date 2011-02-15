UNAME := $(shell uname)

ifeq ($(UNAME), Darwin)
CCLINK=-framework Cocoa -ljpeg -lglfw -framework OpenGL -framework OpenCL
else
CCLINK=-lGL -ljpeg -lglfw
endif

CC=gcc
CFLAGS=-Wall -std=c99 -msse2
SOURCES=opengl/display.c opencl.c gautil.c generate.c jpeg.c vectimg.c
OUTPUT = opengl-ga

all: $(OUTPUT)

$(OUTPUT): $(SOURCES)
	$(CC) $(CFLAGS) $(CCLINK) $(SOURCES) -o $(OUTPUT)

clean:
	rm -f *.o $(OUTPUT)
