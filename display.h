#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "GL/glfw.h"

#define MIN_POLYGONS 1
#define MAX_POLYGONS 255

#define MIN_VERTICES 3
#define MAX_VERTICES 10

#define R_WIDTH 414
#define R_HEIGHT 398

typedef unsigned char byte;

typedef struct vertex {
	int x;
	int y;
} vertex;

typedef struct polygon {
	int num_vertices;
	vertex vertices[MAX_VERTICES];
	byte color[4];
} polygon;

typedef struct vectimg {
    int modified;
    int num_polygons;
    polygon polygons[MAX_POLYGONS];
} vectimg;

void render_vectimg(vectimg *v);
vectimg *gen_random_vectimg();
void gen_random_polygon(polygon *p);
void rasterize_vectimg(vectimg *v, byte *buffer);
long calc_fitness(byte *src, byte *ref);
void display_jpeg(char *filename);
void read_pixels(byte *buffer);
void add_vertex(polygon *p, int x, int y);
void display_pixbuf(byte *buffer, unsigned char inverted);

int random_width();
int random_height();
int random_color();


#endif /* __DISPLAY_H__ */
