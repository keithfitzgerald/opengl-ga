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
vectimg *gen_random_vectimg();
void gen_random_polygon(polygon *p);
void add_vertex(polygon *p, int x, int y);

#endif	/* __VECTIMG_H__ */

