#ifndef __POPULATION_H__
#define __POPULATION_H__

#include "display.h"
#include "pixbuf.h"

typedef struct citizen {
    vectimg *image;
    float fitness;
} citizen;

typedef struct generation {
    citizen citizens[1000];
    int count;
} generation;

void free_generation(generation *g);
generation *create_first_generation(int size, pixbuf *target);

#endif