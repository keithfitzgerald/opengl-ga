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
#include "gltext.h"

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
  
    GLFWwindow *window = setup_display((width * scale) * 2, (height * scale) + HEIGHT_OFFSET);

    Attrib text_attrib = {0};
    GLuint program;

    program = load_program(
        "shaders/text_vertex.glsl", "shaders/text_fragment.glsl");
    text_attrib.program = program;
    text_attrib.position = glGetAttribLocation(program, "position");
    text_attrib.uv = glGetAttribLocation(program, "uv");
    text_attrib.matrix = glGetUniformLocation(program, "matrix");
    text_attrib.sampler = glGetUniformLocation(program, "sampler");

    display_rgb_pixbuf(jpeg->buffer, width, height, 0, HEIGHT_OFFSET, 1);

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

    int generation = 0;
    while (!glfwWindowShouldClose(window)) {
        char line1[64], line2[64], line3[64];

        sprintf(line1, "generation: %d", generation);
        sprintf(line2, "fitness: %ld", prev_fitness);
        sprintf(line3, "polygons: %d", v->num_polygons);
 
        vectimg *c = clone_vectimg(v);
        change_image(c);

        rasterize_vectimg(c, buffer);
        long fitness = calc_fitness(buffer, ref, bufsz);

        if (fitness < prev_fitness || prev_fitness < 0) {
            prev_fitness = fitness;
            free(v);
            v = c;
          
            render_text(&text_attrib, 0, 10, 20, 12, line3, width, height);
            render_text(&text_attrib, 0, 10, 40, 12, line2, width, height);
            render_text(&text_attrib, 0, 10, 60, 12, line1, width, height);
            display_rgb_pixbuf(jpeg->buffer, width, height, width, HEIGHT_OFFSET, 1);

            glfwSwapBuffers(window);
        } else {
            free(c);
        }

        glfwPollEvents();

        generation++;
    }

    free(v);

    _mm_free(buffer);
    _mm_free(ref);
 
    ga_free_jpeg(jpeg);
    glfwDestroyWindow(window);
    glfwTerminate();

    exit(EXIT_SUCCESS);
}
