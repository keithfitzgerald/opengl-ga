CC=gcc
CFLAGS=-Wall -std=c99
CCLINK=-lGL -ljpeg -lglfw
SOURCES=display.c util.c generate.c population.c
OUTPUT = opengl-ga

all: $(OUTPUT)

$(OUTPUT): $(SOURCES)
	$(CC) $(CFLAGS) $(CCLINK) $(SOURCES) -o $(OUTPUT)

clean:
	rm -f *.o $(OUTPUT)
