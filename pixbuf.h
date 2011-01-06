#ifndef __PIXBUF_H__
#define __PIXBUF_H__

#define R_WIDTH 400
#define R_HEIGHT 300

typedef struct pixel {
	float r;
	float g;
	float b;
} pixel;

typedef struct pixbuf {
	pixel data[R_WIDTH][R_HEIGHT];
} pixbuf;

#endif /* __PIXBUF_H__ */