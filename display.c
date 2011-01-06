#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "GL/glfw.h"
#include "pixbuf.h"
#include "display.h"
#include "util.h"

void set_color(polygon *p, float r, float g, float b, float alpha) {
	if (p == NULL) return;

	p->color[0] = r;
	p->color[1] = g;
	p->color[2] = b;
	p->color[3] = alpha;
}

void add_vertex(polygon *p, float x, float y) {
	int idx = p->num_vertices;
	if ((idx + 1) >= MAX_VERTICES) {
		return;
	}

	p->vertices[idx].x = x;
	p->vertices[idx].y = y;	
	p->num_vertices++;
}

float random_width() {
	return (float)(rand() % R_WIDTH);
}

float random_height() {
	return (float)(rand() % R_HEIGHT);
}

float random_color() {
	return (float)(rand()/(RAND_MAX * 1.0)); 
}

vectimg *gen_random_vectimg() {
	int i,j;
	vectimg *v = (vectimg*)malloc(sizeof(vectimg)); 
	for (i = 0; i < MAX_POLYGONS;i++) {
		polygon *p = &v->polygons[i];
		p->num_vertices = 0;

		set_color(p,random_color(),random_color(),random_color(),random_color() /* alpha */);

        // TODO: only triangles for now?
		for (j = 0; j < 3;j++) {
			add_vertex(p,random_width(),random_height());
		}
	}

	return v;
}

void render_vectimg(vectimg *v) {
    int i,j;
    for (i = 0; i < MAX_POLYGONS;i++) {
        polygon p = v->polygons[i];

        glBegin(GL_POLYGON);
        glColor4f(p.color[0], p.color[1], p.color[2], p.color[3]);

        for (j = 0;j < p.num_vertices;j++) {
            vertex v = p.vertices[j];
            glVertex2f(v.x, v.y);
        }

        glEnd();
    }
}

void read_pixel_buffer(pixbuf *buffer) {
    int i,j,idx;

    GLfloat *pixbuf = (GLfloat*)malloc(sizeof(GLfloat) * 3 * R_HEIGHT * R_WIDTH);
    glReadPixels(0, 0, R_WIDTH, R_HEIGHT, GL_RGB, GL_FLOAT, pixbuf);
    
    for (i = 0;i < R_HEIGHT;i++) {
        for (j = 0;j < R_WIDTH;j += 3) {
            idx = (i * R_WIDTH) + j;
            buffer->data[i][j].r = pixbuf[idx];
            buffer->data[i][j].g = pixbuf[idx + 1];
            buffer->data[i][j].b = pixbuf[idx + 2];
        }
    }

    free(pixbuf);
}

void rasterize_vectimg(vectimg *v, pixbuf *buffer) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    render_vectimg(v);
    read_pixel_buffer(buffer);
}

// TODO: wrong place for this?
float calc_fitness(pixbuf *src, pixbuf *ref) {
	int x,y;
	double diff = 0;
	for (y = 0;y < R_HEIGHT;y++) {
		for (x = 0;x < R_WIDTH;x++) {
			diff += fabs(src->data[x][y].r - ref->data[x][y].r);
			diff += fabs(src->data[x][y].g - ref->data[x][y].g);
			diff += fabs(src->data[x][y].b - ref->data[x][y].b);
		}
	}
	return (float)diff;
}

void display_pixbuf(pixbuf *buffer) {
    int x,y;
    pixel *pixel;
    for (y = R_HEIGHT - 1; y >= 0;y--) {
        for (x = R_WIDTH - 1;x >=0 ;x--) {
            glBegin(GL_POINTS);
            pixel = &buffer->data[x][y];
            glColor3f(pixel->r, pixel->g, pixel->b);
            glVertex2f((float)x, (float)(R_HEIGHT - y));
            glEnd();
        }
    }
}

void display_jpeg(char *filename) {
    pixbuf *buffer = (pixbuf*)malloc(sizeof(pixbuf));
    read_jpeg(filename, buffer);
    display_pixbuf(buffer);
    free(buffer);
}