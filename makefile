CC=gcc
CFLAGS=-Wall
CCLINK=-lGL -ljpeg -lglfw
SOURCES=pixbuf.c display.c util.c generate.c population.c
OUTPUT = opengl-ga

all: $(OUTPUT)

$(OUTPUT): $(SOURCES)
	$(CC) $(CFLAGS) $(CCLINK) $(SOURCES) -o $(OUTPUT)

clean:
	rm -f *.o $(OUTPUT)
