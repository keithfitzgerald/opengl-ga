#ifndef __VECTIMG_H__
#define	__VECTIMG_H__

#include "gautil.h"

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
    int width;
    int height;
} vectimg;

void render_vectimg(vectimg *v);
void print_vectimg(char *name, vectimg *v);
vectimg *gen_random_vectimg(int num_polygons, int width, int height);
void add_vertex(polygon *p, int x, int y);
vectimg *clone_vectimg(vectimg *v);
void change_image(vectimg *v);

#endif	/* __VECTIMG_H__ */

