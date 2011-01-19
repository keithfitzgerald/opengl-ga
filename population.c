#include <stdlib.h>
#include <stdio.h>
#include "population.h"
#include "display.h"

int cmp_by_fitness(const void *a, const void *b) 
{ 
    return ((citizen*)a)->fitness - ((citizen*)b)->fitness;
} 

generation *create_first_generation(int size, byte *target) {
    int i;
    generation *gen = (generation*)malloc(sizeof(generation));
    gen->count = 100;
    for (i = 0;i < size;i++) {
        vectimg *v = gen_random_vectimg();
        byte *buffer = (byte*)malloc(sizeof(byte) * 3 * R_HEIGHT * R_WIDTH);
        rasterize_vectimg(v, buffer);

        gen->citizens[i].image = v;
        gen->citizens[i].fitness = calc_fitness(buffer, target);

        free(buffer);
    }

    return gen;
}

void free_generation(generation *gen) {
    int i;
    for (i = 0;i < gen->count;i++) {
        free(gen->citizens[i].image);
    }
    free(gen);
}


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
     int ridx = rand() % np;
 
     if ((np - 1) > MIN_POLYGONS) {
        for (i = ridx;i < np;i++) {
            v->polygons[i] = v->polygons[i+1];
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
     add_vertex(p,random_width(),random_height());
   } 
}

void remove_vertex(polygon *p) {
     int i;
     int nv = p->num_vertices;
     // TODO: distribution correct?
     int ridx = rand() % nv;
 
     if ((nv - 1) > MIN_VERTICES) {
        for (i = ridx;i < nv;i++) {
            p->vertices[i] = p->vertices[i+1];
        }
        p->num_vertices--; 
     }
}

int ga_max(int a, int b) {
   return ((a)>(b)?(a):(b));
}

int ga_min(int a, int b) {
   return ((a)>(b)?(b):(a));
}

int ga_random(int min, int max) {
   return (rand() % (max - min)) + min;
}

void change_color(vectimg *v,polygon *p) {
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

void change_polygon(vectimg *v,polygon *p) {
    
    if ((rand() % ADD_VERTEX_RATE) == 1) {
       add_new_vertex(p);
       v->modified = 1;
    }

    if ((rand() % REMOVE_VERTEX_RATE) == 1) {
       remove_vertex(p);
       v->modified = 1;
    }

    change_color(v,p);

    int nv = p->num_vertices;
    for (int i = 0;i < nv;i++) {
        if ((rand() % MOVE_VERTEX_MAX) == 1) {
           p->vertices[i].x = random_width();
           p->vertices[i].y = random_height();   
           v->modified = 1;
        }

        int x = p->vertices[i].x;
        int y = p->vertices[i].y;

        if ((rand() % MOVE_VERTEX_MID) == 1) {
           p->vertices[i].x = ga_min(ga_max(0,x + ga_random(-20,20)),R_WIDTH);  
           p->vertices[i].y = ga_min(ga_max(0,y + ga_random(-20,20)),R_HEIGHT); 
           v->modified = 1;
        }
 
        if ((rand() % MOVE_VERTEX_MIN) == 1) {
           p->vertices[i].x = ga_min(ga_max(0,x + ga_random(-3,3)),R_WIDTH);   
           p->vertices[i].y = ga_min(ga_max(0,y + ga_random(-3,3)),R_HEIGHT);   
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

    for (int i = 0;i < v->num_polygons;i++) {
       change_polygon(v,&v->polygons[i]);
    }   
}


