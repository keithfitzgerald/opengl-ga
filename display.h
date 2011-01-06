#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "pixbuf.h"

#define MAX_POLYGONS 10
#define MAX_VERTICES 10

typedef struct vertex {
	float x;
	float y;
} vertex;

typedef struct polygon {
	int num_vertices;
	vertex vertices[MAX_VERTICES];
	float color[4];
} polygon;

typedef struct vectimg {
    polygon polygons[MAX_POLYGONS];
} vectimg;

void render_vectimg(vectimg *v);
vectimg *gen_random_vectimg();
void rasterize_vectimg(vectimg *v, pixbuf *buffer);
float calc_fitness(pixbuf *src, pixbuf *ref);
void display_jpeg(char *filename);
void read_pixel_buffer(pixbuf *buffer);

#endif /* __DISPLAY_H__ */