#ifndef __BITMAP_H__
#define __BITMAP_H__

#include "polygon.h"

typedef struct bitmap {
	int height;
	int width;
	pixel data[500][500];
} bitmap;

#endif
