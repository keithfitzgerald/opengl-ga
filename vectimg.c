#include <stdlib.h>
#include <stdio.h>
#include "gautil.h"
#include "vectimg.h"

void add_vertex(polygon *p, int x, int y) {
    int idx = p->num_vertices;
    if ((idx + 1) > MAX_VERTICES) {
        return;
    }

    p->vertices[idx].x = x;
    p->vertices[idx].y = y;
    p->num_vertices++;
}

void set_color(polygon *p, byte r, byte g, byte b, byte alpha) {
    if (!p) return;

    p->color[0] = r;
    p->color[1] = g;
    p->color[2] = b;
    p->color[3] = alpha;
}

void gen_random_polygon(polygon *p, int width, int height) {
    p->num_vertices = 0;
    set_color(p, random_color(), random_color(),
              random_color(), random_alpha());

    int ax = random_width(width);
    int ay = random_height(height);

    for (int i = 0; i < MIN_VERTICES; i++) {
        int x = ga_min(ga_max(0, ax + ga_rand_range(-3,3)),width);
        int y = ga_min(ga_max(0, ay + ga_rand_range(-3,3)),height);
        add_vertex(p,x,y);
    }
}

void add_polygon(vectimg *v) {
    int np = v->num_polygons;
    if (np < MAX_POLYGONS) {
        gen_random_polygon(&v->polygons[np],v->width,v->height);
        v->num_polygons++;
    }
}

void remove_polygon(vectimg *v) {
    int i;
    int np = v->num_polygons;
    int ridx = ga_rand_range(1,np);

    if ((np - 1) > MIN_POLYGONS) {
        for (i = ridx; i < np; i++) {
            v->polygons[i] = v->polygons[i + 1];
        }
        v->num_polygons--;
    }
}

void move_polygon(vectimg *v) {
    int np = v->num_polygons;
    int from = ga_rand_range(1,np);
    int to = ga_rand_range(1,np);

    polygon tmp = v->polygons[from];
    v->polygons[from] = v->polygons[to];
    v->polygons[to] = tmp;
}

void add_new_vertex(polygon *p, int width, int height) {
    if (p->num_vertices < MAX_VERTICES) {
        add_vertex(p, random_width(width), random_height(height));
    }
}

void remove_vertex(polygon *p) {
    int i;
    int nv = p->num_vertices;
    int ridx = ga_rand_range(1,nv);

    if ((nv - 1) > MIN_VERTICES) {
        for (i = ridx; i < nv; i++) {
            p->vertices[i] = p->vertices[i + 1];
        }
        p->num_vertices--;
    }
}

vectimg *gen_random_vectimg(int num_polygons, int width, int height) {
    vectimg *v = (vectimg*) malloc(sizeof (vectimg));
    v->num_polygons = num_polygons;
    for (int i = 0; i < num_polygons; i++) {
        polygon *p = &v->polygons[i];
        gen_random_polygon(p, width, height);
    }

    v->width = width;
    v->height = height;
    v->modified = 0;

    return v;
}

void change_color(vectimg *v, polygon *p) {
    
    if (rand() % CHANGE_COLOR_RATE == 1) {
        p->color[0] = random_color();
        v->modified = 1;
    }

    if (rand() % CHANGE_COLOR_RATE == 1) {
        p->color[1] = random_color();
        v->modified = 1;
    }

    if (rand() % CHANGE_COLOR_RATE == 1) {
        p->color[2] = random_color();
        v->modified = 1;
    }

    if (rand() % CHANGE_COLOR_RATE == 1) {
        p->color[3] = random_alpha();
        v->modified = 1;
    }
}

void change_polygon(vectimg *v, polygon *p) {

    if ((ga_rand() % ADD_VERTEX_RATE) == 1) {
        add_new_vertex(p, v->width, v->height);
        v->modified = 1;
    }

    if ((ga_rand() % REMOVE_VERTEX_RATE) == 1) {
        remove_vertex(p);
        v->modified = 1;
    }

    change_color(v, p);

    int nv = p->num_vertices;
    for (int i = 0; i < nv; i++) {
        if ((ga_rand() % MOVE_VERTEX_MAX) == 1) {
            p->vertices[i].x = random_width(v->width);
            p->vertices[i].y = random_height(v->height);
            v->modified = 1;
        }

        int x = p->vertices[i].x;
        int y = p->vertices[i].y;

        // TODO: this could be cleaned up
        if ((ga_rand() % MOVE_VERTEX_MID) == 1) {
            p->vertices[i].x = ga_min(ga_max(0, x + ga_rand_range(-20, 20)), v->width);
            p->vertices[i].y = ga_min(ga_max(0, y + ga_rand_range(-20, 20)), v->height);
            v->modified = 1;
        }

        if ((ga_rand() % MOVE_VERTEX_MIN) == 1) {
            p->vertices[i].x = ga_min(ga_max(0, x + ga_rand_range(-3, 3)), v->width);
            p->vertices[i].y = ga_min(ga_max(0, y + ga_rand_range(-3, 3)), v->height);
            v->modified = 1;
        }
    }
}

void change_image(vectimg *v) {

    if ((ga_rand() % ADD_POLYGON_RATE) == 1) {
        add_polygon(v);
        v->modified = 1;
    }

    if ((ga_rand() % REMOVE_POLYGON_RATE) == 1) {
        remove_polygon(v);
        v->modified = 1;
    }

    if ((ga_rand() % MOVE_POLYGON_RATE) == 1) {
        move_polygon(v);
        v->modified = 1;
    }

    for (int i = 0; i < v->num_polygons; i++) {
        change_polygon(v, &v->polygons[i]);
    }
}

vectimg *clone_vectimg(vectimg *v) {
    vectimg *c = (vectimg*)malloc(sizeof(vectimg));
    int sz = v->num_polygons; 
    for (int i = 0; i < sz;i++) {
        c->polygons[i] = v->polygons[i];
    }
    c->num_polygons = v->num_polygons;
    c->width = v->width;
    c->height = v->height;
    c->modified = 0;

    return c;
}

void print_vectimg(char *name, vectimg *v) {
    printf("vectimg[%s]: polygons=%d width=%d height=%d modified=%d\n",
            name, v->num_polygons, v->width, v->height, v->modified);
    for (int i = 0; i < v->num_polygons; i++) {
        polygon *p = &v->polygons[i];
        byte *color = p->color;
        printf("  polygon[%d]: vertices=%d r=%d g=%d b=%d a=%d\n", i,
                p->num_vertices, color[0], color[1], color[2], color[3]);

        for (int j = 0; j < p->num_vertices; j++) {
            vertex *v = &p->vertices[j];
            printf("    vertex[%d]: x=%d y=%d\n", j, v->x,v->y);
        }
    }
}
