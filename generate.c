#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#ifdef _MSC_VER
  #include <emmintrin.h>
#else
  #include <xmmintrin.h>
#endif
#include <glfw/glfw3.h>
#include "display.h"
#include "gautil.h"
#include "jpeg.h"
#include "vectimg.h"

#define BUF_ALIGN_WIDTH 16

int main(int argc, char **argv) {
  
    if (argc != 3) {
        printf("usage: opengl-ga [image.jpg] [scale (int)]\n");
        return EXIT_FAILURE;
    }

    srand((unsigned int) time(0));
	
    int scale = 1; // atoi(argv[2]);
    ga_jpeg *jpeg = ga_read_jpeg(argv[1]);
    int width = jpeg->width;
    int height = jpeg->height;
  
    GLFWwindow *window = setup_display((width * scale) * 2, height * scale);

    // TODO: make it so the ref image is side-by-side
    display_rgb_pixbuf(jpeg->buffer, width, height, 0, 0, 1);

    //ga_free_jpeg(jpeg);

    // align buffer to BUF_ALIGN_WIDTH
    int numpix = (width * height * 3);
    int bufsz = numpix + (BUF_ALIGN_WIDTH - (numpix % BUF_ALIGN_WIDTH));

    printf("alignment %d bytes, buffer size: %d\n",BUF_ALIGN_WIDTH, bufsz);
    
    byte *ref = (byte*)_mm_malloc(sizeof (byte) * bufsz, BUF_ALIGN_WIDTH);
    read_pixels(ref, width, height);

    long prev_fitness = -1;
    int selected = 0;
    vectimg *v = gen_random_vectimg(1,width, height);
    //print_vectimg("v",v);

    byte *buffer = (byte*)_mm_malloc(sizeof (byte) * bufsz, BUF_ALIGN_WIDTH);

    int i = 0, iters = 0;
    while (1) {
        iters++;
        if (iters % 2500 == 0) {
            int points = 0;
            for (int x = 0;x < v->num_polygons;x++) {
                points += v->polygons[x].num_vertices;
            }

            printf("generation: %d selected: %d fitness: %ld points %d polygons: %d iters: %d\n",
                    i, selected, prev_fitness, points,
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
        if (fitness < prev_fitness || prev_fitness < 0) {
            prev_fitness = fitness;
            free(v);
            v = c;
            render_vectimg(c, scale);
            display_rgb_pixbuf(jpeg->buffer, width, height, width, 0, 1);
            glfwSwapBuffers(window);
            selected++;

        } else {
            free(c);
        }

        i++;
    }
    printf("\n");

    ga_free_jpeg(jpeg);

    free(v);
    _mm_free(buffer);
    _mm_free(ref);
    glfwTerminate();

    exit(EXIT_SUCCESS);
}
