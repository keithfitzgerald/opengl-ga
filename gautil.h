#ifndef __GAUTIL_H__
#define __GAUTIL_H__

#include <stdint.h>

#define byte uint8_t

#define MIN_POLYGONS 1
//#define MAX_POLYGONS 50
#define MAX_POLYGONS 100

#define MIN_VERTICES 3
#define MAX_VERTICES 10

#define ADD_POLYGON_RATE 35
#define REMOVE_POLYGON_RATE 75
#define MOVE_POLYGON_RATE 35

#define ADD_VERTEX_RATE 75
#define REMOVE_VERTEX_RATE 75
#define MOVE_VERTEX_RATE 35

#define MOVE_VERTEX_MAX 75
#define MOVE_VERTEX_MID 75
#define MOVE_VERTEX_MIN 75

#define CHANGE_COLOR_RATE 75

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

long calc_fitness(byte *src, byte *ref, int bufsz);

int random_width(int width);
int random_height(int height);
int random_color();
int random_alpha();
int random_vertices();
int random_polygons();
int ga_rand();
int ga_rand_range(int a, int b);
int ga_max(int a, int b);
int ga_min(int a, int b);

#endif /* __GAUTIL_H__ */
