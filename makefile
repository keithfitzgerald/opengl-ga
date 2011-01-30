CC=gcc
CFLAGS=-Wall -std=c99 -msse2
CCLINK=-lGL -ljpeg -lglfw
SOURCES=opengl/display.c gautil.c generate.c jpeg.c vectimg.c
OUTPUT = opengl-ga

all: $(OUTPUT)

$(OUTPUT): $(SOURCES)
	$(CC) $(CFLAGS) $(CCLINK) $(SOURCES) -o $(OUTPUT)

clean:
	rm -f *.o $(OUTPUT)
