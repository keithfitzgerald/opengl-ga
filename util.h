#ifndef __UTIL_H__
#define __UTIL_H__

#include "bitmap.h"

#define R_HEIGHT 640
#define R_WIDTH 480

float getRandomPointX();
float getRandomPointY();
float getRandomColor();

int readJPEG(char *filename, bitmap *result);
polygon* gen_random_image();
void free_image(polygon *image);

#endif
