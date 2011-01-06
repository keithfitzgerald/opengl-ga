#include <math.h>
#include "bitmap.h"

float calc_fitness(bitmap *image, bitmap *ref) {
	int x,y;
	double diff = 0;
	for (y = 0;y < image->height;y++) {
		for (x = 0;x < image->width;x++) {
			diff += fabs(image->data[x][y].r - ref->data[x][y].r);
			diff += fabs(image->data[x][y].g - ref->data[x][y].g);
			diff += fabs(image->data[x][y].b - ref->data[x][y].b);
		}
	}

	return (float)diff;
}



