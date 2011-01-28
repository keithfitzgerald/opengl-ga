#include <time.h>
#include <xmmintrin.h>

srand((unsigned int) time(0));

int random_width(int width) {
    return ga_rand() % width;
}

int random_height(int height) {
    return ga_rand() % height;
}

int random_vertices() {
    return (ga_rand() % (MAX_VERTICES - MIN_VERTICES)) +MIN_VERTICES;
}

int random_polygons() {
    // TODO: this can return zero
    return (ga_rand() + MAX_POLYGONS) % MAX_POLYGONS;
}

int random_color() {
    return ga_rand() % 255;
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
    return (ga_rand() % (max - min)) +min;
}

void ga_read_ref_img(char *imgname, ga_context* context) {

}

long calc_fitness(byte *src, byte *ref) {

    long diff = 0;
    int sz = R_BUFSZ;

    // TODO: not going to compile on msvc
    uint16_t xc[8] __attribute__((aligned(16))) = {0, 0, 0, 0, 0, 0, 0, 0};

    for (int i = 0; i < sz; i += 16) {
        __m128i a = _mm_load_si128((__m128i*) (src + i));
        __m128i b = _mm_load_si128((__m128i*) (ref + i));
        __m128i c = _mm_sad_epu8(a, b);
        _mm_store_si128((__m128i*) & xc, c);
        diff += xc[0] + xc[4];
    }
    return diff;
}

void print_vectimg(char *name, vectimg *v) {
    printf("vectimg[%s]: %d polygons\n", name, v->num_polygons);
    int sz = v->num_polygons;
    for (int i = 0; i < sz; i++) {
        printf("    polygon[%d]: %d vertices\n", i, v->polygons[i].num_vertices);
    }
}
