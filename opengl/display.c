#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "GL/glfw.h"
#include "display.h"
#include "util.h"
#include <xmmintrin.h>
#include <stdint.h>

void set_color(polygon *p, byte r, byte g, byte b, byte alpha) {
	if (p == NULL) return;

	p->color[0] = r;
	p->color[1] = g;
	p->color[2] = b;
	p->color[3] = alpha;
}

void add_vertex(polygon *p, int x, int y) {
	int idx = p->num_vertices;
	if ((idx + 1) > MAX_VERTICES) {
		return;
	}

	p->vertices[idx].x = x;
	p->vertices[idx].y = y;	
	p->num_vertices++;
}

int random_width() {
	return rand() % R_WIDTH;
}

int random_height() {
	return rand() % R_HEIGHT;
}

int random_vertices() {
    return (rand() % (MAX_VERTICES - MIN_VERTICES)) + MIN_VERTICES; 
}

int random_polygons() {
    // TODO: this can return zero
    return (rand() + MAX_POLYGONS) % MAX_POLYGONS;
}

int random_color() {
	return (GLint)(rand() % 255); 
}

void gen_random_polygon(polygon *p) {
		p->num_vertices = 0;
		set_color(p,random_color(),random_color(),random_color(),random_color() /* alpha */);

        int num_vertices = random_vertices();
		for (int j = 0; j < num_vertices;j++) {
			add_vertex(p,random_width(),random_height());
		}
}

vectimg *gen_random_vectimg() {
	vectimg *v = (vectimg*)malloc(sizeof(vectimg)); 
    int sz = MIN_POLYGONS;
    v->num_polygons = sz;
	for (int i = 0; i < sz;i++) {
		polygon *p = &v->polygons[i];
        gen_random_polygon(p);
	}

	return v;
}

void render_vectimg(vectimg *v) {
    for (int i = 0; i < v->num_polygons;i++) {
        polygon p = v->polygons[i];

        glBegin(GL_POLYGON);
        // TODO: why doesn't this work?
        //glColor4iv(p.color);
        
        glColor4ub(p.color[0],p.color[1],p.color[2],p.color[3]);

        for (int j = 0;j < p.num_vertices;j++) {
            vertex v = p.vertices[j];
            glVertex2i(v.x, v.y);
        }

        glEnd();
    }
}

void read_pixels(byte *buffer) {
    glReadPixels(0, 0, R_WIDTH, R_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, buffer);
}

void rasterize_vectimg(vectimg *v, byte *buffer) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    render_vectimg(v);
    read_pixels(buffer);
}

// TODO: wrong place for this?
long calc_fitness(byte *src, byte *ref) {

    long diff = 0;
    int sz = R_BUFSZ;
    
    uint16_t xc[8] __attribute__((aligned(16))) = {0,0,0,0,0,0,0,0};

    for (int i = 0;i < sz;i += 16) {
        __m128i a = _mm_load_si128((__m128i*)(src + i));
        __m128i b = _mm_load_si128((__m128i*)(ref + i));
        __m128i c = _mm_sad_epu8(a,b);
        _mm_store_si128((__m128i*)&xc, c);
        diff += xc[0] + xc[4];
      //diff += abs(src[i] - ref[i]);
	}
	return diff;
}

void display_pixbuf(byte *buffer, unsigned char inverted) {
    int base = inverted ? R_HEIGHT - 1 : 0;
    int stride = R_WIDTH * 3;    
    for (int y = 0;y < R_HEIGHT;y++) {
      for (int x = 0;x < stride;x += 3) {
            int i = ((base - y) * stride) + x;
            //printf("out pixel [%d,%d]: R=%d,G=%d,B=%d\n",x/3,y,buffer[i],buffer[i+1],buffer[i+2]);
            //fflush(stdout);
            glBegin(GL_POINTS);
            glColor3ub(buffer[i],buffer[i+1], buffer[i+2]);
            glVertex2i(x/3,y);
            glEnd();
          }
    }
}

void display_jpeg(char *filename) {
    byte *buffer = ga_read_jpeg(filename);
    display_pixbuf(buffer,1);
    free(buffer);
}
