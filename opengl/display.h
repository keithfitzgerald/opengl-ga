#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "../gautil.h"

// #define R_BUFSZ 495558  // 16 bit aligned buffer

void rasterize_vectimg(vectimg *v, byte *buffer);

void display_jpeg(char *filename);
void read_pixels(byte *buffer, int width, int height);
void display_rgb_pixbuf(byte *buffer, int width, int height, 
                        unsigned char inverted);

#endif /* __DISPLAY_H__ */
