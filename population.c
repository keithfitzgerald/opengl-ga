#include <stdlib.h>
#include "population.h"
#include "pixbuf.h"
#include "display.h"

/*
polygon *mate(polygon *image1, polygon *image2) {
    float val = (float)(rand()/(RAND_MAX * 1.0));
    polygon *image = (polygon*)malloc(sizeof(polygon) * MAX_POLYGONS);
    if (val > 0.5) {
        image->
    }


}
*/

generation *create_first_generation(int size, pixbuf *target) {
    int i;
    generation *gen = (generation*)malloc(sizeof(generation));
    gen->count = 100;
    for (i = 0;i < size;i++) {
        vectimg *v = gen_random_vectimg();
        pixbuf *buffer = (pixbuf*)malloc(sizeof(pixbuf));
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


