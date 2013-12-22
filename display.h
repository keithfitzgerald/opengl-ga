#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include <glfw/glfw3.h>
#include "vectimg.h"
#include "gautil.h"

GLFWwindow* setup_display(int width, int height);
void rasterize_vectimg(vectimg *v, byte *buffer);

void display_jpeg(char *filename);
void read_pixels(byte *buffer, int width, int height);
void display_rgb_pixbuf(byte *buffer, int width, int height, 
                        int xpos, int ypos, unsigned char inverted);
void render_vectimg(vectimg *v, int scale);

#endif /* __DISPLAY_H__ */
