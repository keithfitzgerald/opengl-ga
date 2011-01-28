#ifndef __GAUTIL_H__
#define __GAUTIL_H__

#define byte uint8_t

#define MIN_POLYGONS 1
#define MAX_POLYGONS 255

#define MIN_VERTICES 3
#define MAX_VERTICES 10

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

typedef struct ga_context {
    byte *ref; // RGBA array of the refrence image
    byte *cur; // RGBA array of the currrent comparison image
    int fitness; // fitness (distance) between the two images
    int width;
    int height;
} ga_context;

void ga_read_ref_img(char *imgname, ga_context* context);
long calc_fitness(byte *src, byte *ref);


int random_width();
int random_height();
int random_color();
int random_vertices();
int ga_rand();
int ga_rand_range(int a, int b);
int ga_max(int a, int b);
int ga_min(int a, int b);

#endif /* __GAUTIL_H__ */
