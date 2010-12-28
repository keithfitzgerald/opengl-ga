CC=gcc
CFLAGS=-Wall
CCLINK=-lGL -ljpeg -lglfw
SOURCES=bitmap.c polygon.c util.c render.c
OUTPUT = opengl-ga

all: $(OUTPUT)

$(OUTPUT): $(SOURCES)
	$(CC) $(CFLAGS) $(CCLINK) $(SOURCES) -o $(OUTPUT)

clean:
	rm -f *.o $(OUTPUT)
