#include "gautil.h"
#include "vectimg.h"

void add_polygon(vectimg *v) {
    int np = v->num_polygons;
    if (np < MAX_POLYGONS) {
        gen_random_polygon(&v->polygons[np]);
        v->num_polygons++;
    }
}

void remove_polygon(vectimg *v) {
    int i;
    int np = v->num_polygons;
    // TODO: distribution correct?
    int ridx = random() % np;

    if ((np - 1) > MIN_POLYGONS) {
        for (i = ridx; i < np; i++) {
            v->polygons[i] = v->polygons[i + 1];
        }
        v->num_polygons--;
    }
}

void move_polygon(vectimg *v) {
    remove_polygon(v);
    add_polygon(v);
}

void add_new_vertex(polygon *p) {
    if (p->num_vertices < MAX_VERTICES) {
        add_vertex(p, random_width(), random_height());
    }
}

void remove_vertex(polygon *p) {
    int i;
    int nv = p->num_vertices;
    // TODO: distribution correct?
    int ridx = rand() % nv;

    if ((nv - 1) > MIN_VERTICES) {
        for (i = ridx; i < nv; i++) {
            p->vertices[i] = p->vertices[i + 1];
        }
        p->num_vertices--;
    }
}

void set_color(polygon *p, byte r, byte g, byte b, byte alpha) {
    if (!p) return;

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

void gen_random_polygon(polygon *p) {
    p->num_vertices = 0;
    set_color(p, random_color(), random_color(),
            random_color(), random_color() /* alpha */);

    int num_vertices = random_vertices();
    for (int j = 0; j < num_vertices; j++) {
        add_vertex(p, random_width(), random_height());
    }
}

vectimg *gen_random_vectimg() {
    vectimg *v = (vectimg*) malloc(sizeof (vectimg));
    int sz = MIN_POLYGONS;
    v->num_polygons = sz;
    for (int i = 0; i < sz; i++) {
        polygon *p = &v->polygons[i];
        gen_random_polygon(p);
    }

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
        p->color[3] = random_color();
        v->modified = 1;
    }
}

void change_polygon(vectimg *v, polygon *p) {

    if ((rand() % ADD_VERTEX_RATE) == 1) {
        add_new_vertex(p);
        v->modified = 1;
    }

    if ((rand() % REMOVE_VERTEX_RATE) == 1) {
        remove_vertex(p);
        v->modified = 1;
    }

    change_color(v, p);

    int nv = p->num_vertices;
    for (int i = 0; i < nv; i++) {
        if ((rand() % MOVE_VERTEX_MAX) == 1) {
            p->vertices[i].x = random_width();
            p->vertices[i].y = random_height();
            v->modified = 1;
        }

        int x = p->vertices[i].x;
        int y = p->vertices[i].y;

        // TODO: this could be cleaned up
        if ((rand() % MOVE_VERTEX_MID) == 1) {
            p->vertices[i].x = ga_min(ga_max(0, x + ga_rand_range(-20, 20)), v->width);
            p->vertices[i].y = ga_min(ga_max(0, y + ga_rand_range(-20, 20)), v->height);
            v->modified = 1;
        }

        if ((rand() % MOVE_VERTEX_MIN) == 1) {
            p->vertices[i].x = ga_min(ga_max(0, x + ga_rand_range(-3, 3)), v->width);
            p->vertices[i].y = ga_min(ga_max(0, y + ga_rand_range(-3, 3)), v->height);
            v->modified = 1;
        }
    }
}

void change_image(vectimg *v) {

    if ((rand() % ADD_POLYGON_RATE) == 1) {
        add_polygon(v);
        v->modified = 1;
    }

    if ((rand() % REMOVE_POLYGON_RATE) == 1) {
        remove_polygon(v);
        v->modified = 1;
    }

    if ((rand() % MOVE_POLYGON_RATE) == 1) {
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
    c->modified = 0;

    return c;
}
