#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <xmmintrin.h>
#include "GL/glfw.h"
#include "opengl/display.h"
#include "gautil.h"
#include "jpeg.h"
#include "vectimg.h"
#include "unistd.h"

#define BUF_ALIGN_WIDTH 16

int main(int argc, char **argv) {

    srand((unsigned int) time(0));

    ga_jpeg *jpeg = ga_read_jpeg(argv[1]);
    int width = jpeg->width;
    int height = jpeg->height;

    setup_display(width, height);

    // TODO: make it so the ref image is side-by-side
    display_rgb_pixbuf(jpeg->buffer, width, height, 1);

    ga_free_jpeg(jpeg);

    // align buffer to BUF_ALIGN_WIDTH
    int numpix = (width * height * 3);
    int bufsz = numpix + (BUF_ALIGN_WIDTH - (numpix % BUF_ALIGN_WIDTH));

    printf("alignment %d bytes, buffer size: %d\n",BUF_ALIGN_WIDTH, bufsz);
    
    byte *ref = _mm_malloc(sizeof (byte) * bufsz, BUF_ALIGN_WIDTH);
    read_pixels(ref, width, height);

    long prev_fitness = -1;
    int selected = 0;
    vectimg *v = gen_random_vectimg(1,width, height);
    //print_vectimg("v",v);

    byte *buffer = _mm_malloc(sizeof (byte) * bufsz, BUF_ALIGN_WIDTH);

    rasterize_vectimg(v, buffer);

    int i = 0, iters = 0;
    while (1) {
        iters++;
        if (iters % 2500 == 0) {
            printf("generation: %d selected: %d fitness: %ld polygons: %d iters: %d\n",
                    i, selected, prev_fitness,
                    v->num_polygons, iters);
            fflush(stdout);
        }
        vectimg *c = clone_vectimg(v);
        change_image(c);

        if (!c->modified) {
            free(c);
            continue;
        }

        rasterize_vectimg(c, buffer);

        long fitness = calc_fitness(buffer, ref, bufsz);
        //printf("fitness: %ld\n",fitness);
        if (fitness < prev_fitness || prev_fitness < 0) {
            prev_fitness = fitness;
            free(v);
            v = c;
            glfwSwapBuffers();
            selected++;

        } else {
            free(c);
        }


        i++;
    }
    printf("\n");

    free(v);
    _mm_free(buffer);
    _mm_free(ref);
    glfwTerminate();

    exit(EXIT_SUCCESS);
}
