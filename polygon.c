#include <stdlib.h>
#include <stdio.h>
#include "polygon.h"

void set_color(polygon *p, float r, float g, float b, float alpha) {
	if (p == NULL) return;

	p->color.r = r;
	p->color.g = g;
	p->color.b = b;
	p->color.alpha = alpha;
}

void add_vertex(polygon *p,float x, float y) {
	int idx = p->num_vertices;

	if ((idx + 1) >= MAX_VERTICES) {
		return;
	}

	printf("adding vertex %d: x=%f,y=%f\n",idx,x,y);

	p->vertices[idx].x = x;
	p->vertices[idx].y = y;
	
	p->num_vertices++;
}