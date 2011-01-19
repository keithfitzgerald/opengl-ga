#ifndef __POPULATION_H__
#define __POPULATION_H__

#include "display.h"

#define ADD_POLYGON_RATE 350 
#define REMOVE_POLYGON_RATE 750
#define MOVE_POLYGON_RATE 350

#define ADD_VERTEX_RATE 750
#define REMOVE_VERTEX_RATE 750
#define MOVE_VERTEX_RATE 350

#define MOVE_VERTEX_MAX 750
#define MOVE_VERTEX_MID 750
#define MOVE_VERTEX_MIN 750

#define CHANGE_COLOR_RATE 750


/* defaults from evolisa */
/*
#define ADD_POLYGON_RATE 700 
#define REMOVE_POLYGON_RATE 1500
#define MOVE_POLYGON_RATE 700

#define ADD_VERTEX_RATE 1500
#define REMOVE_VERTEX_RATE 1500
#define MOVE_VERTEX_RATE 700 

#define MOVE_VERTEX_MAX 1500
#define MOVE_VERTEX_MID 1500
#define MOVE_VERTEX_MIN 1500

#define CHANGE_COLOR_RATE 1500

*/

typedef struct citizen {
    vectimg *image;
    float fitness;
} citizen;

typedef struct generation {
    citizen citizens[1000];
    int count;
} generation;

void free_generation(generation *g);
generation *create_first_generation(int size, byte *target);
void change_image(vectimg *v);

#endif
