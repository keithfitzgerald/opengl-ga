#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#ifdef _MSC_VER
  #include <emmintrin.h>
#else
  #include <xmmintrin.h>
#endif
#include "gautil.h"
#include "vectimg.h"

int random_width(int width) {
    return ga_rand() % width;
}

int random_height(int height) {
    return ga_rand() % height;
}

int random_vertices() {
    return ga_rand_range(MIN_VERTICES, MAX_VERTICES);
}

int random_polygons() {
    return ga_rand_range(MIN_POLYGONS, MAX_POLYGONS);
}

int random_color() {
    return ga_rand_range(0,255);
}

int random_alpha() {
    return ga_rand_range(30,60);
}

int ga_rand() {
    return rand();
}

int ga_max(int a, int b) {
    return ((a)>(b) ? (a) : (b));
}

int ga_min(int a, int b) {
    return ((a)>(b) ? (b) : (a));
}

int ga_rand_range(int min, int max) {
    if (min == max) {
        return min;
    }

    return (ga_rand() % (max - min)) +min;
}

long calc_fitness(byte *src, byte *ref, int bufsz) {

    long diff = 0;

    #ifdef _MSC_VER
        __declspec(align(16)) uint16_t xc[8]  = {0, 0, 0, 0, 0, 0, 0, 0};
    #else
        uint16_t xc[8] __attribute__((aligned(16))) = {0, 0, 0, 0, 0, 0, 0, 0};
    #endif

    for (int i = 0; i < bufsz; i += 16) {
       
	    __m128i a = _mm_load_si128((__m128i*) (src + i));
        __m128i b = _mm_load_si128((__m128i*) (ref + i));
        __m128i c = _mm_sad_epu8(a, b);
        _mm_store_si128((__m128i*) & xc, c);
        diff += xc[0] + xc[4];
    }
    return diff;
}

