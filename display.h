#ifndef __POLYGON_H__
#define __POLYGON_H__

#define MAX_POLYGONS 10
#define MAX_VERTICES 10

#define C_RED 0
#define C_GREEN 1
#define C_BLUE 2
#define C_ALPHA 3

typedef struct pixel {
	float r;
	float g;
	float b;
	float alpha;
} pixel;

typedef struct vertex {
	float x;
	float y;
} vertex;

typedef struct polygon {
	int num_vertices;
	vertex vertices[MAX_VERTICES];
	pixel color;
} polygon;

void set_color(polygon *p, float r, float g, float b, float alpha);
void add_vertex(polygon *p, float x, float y);

#endif
