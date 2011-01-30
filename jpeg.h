#ifndef __JPEG_H__
#define	__JPEG_H__

#include "gautil.h"

typedef struct ga_jpeg {
    byte *buffer;
    int width;
    int height;
} ga_jpeg;

ga_jpeg *ga_read_jpeg(char *filename);
void ga_free_jpeg(ga_jpeg *jpeg);

#endif	/* __JPEG_H__ */

