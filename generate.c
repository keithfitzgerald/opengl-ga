#ifdef __OPENGL__ 
#include "GL/glfw.h"
#include "opengl/display.h"
#elif __OPENGL_ES__
#include "GLes/glfw.h"
#include "opengl-es/display.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include <xmmintrin.h>

int main(int argc, char **argv) {
 
    setup_display();

    display_jpeg("firefox.jpg");
   
    byte *ref = _mm_malloc(sizeof(byte) * R_BUFSZ,16);
    read_pixels(ref);
  
    long prev_fitness = -1;
    int selected = 0;
    vectimg *v = gen_random_vectimg();
 
    byte *buffer = _mm_malloc(sizeof(byte) * R_BUFSZ,16);
 
    int i = 0, iters = 0;
    while(1) {
       iters++;
       if (iters % 2500 == 0) { 
         printf("generation: %d selected: %d fitness: %ld polygons: %d iters: %d\n",i,selected,prev_fitness,v->num_polygons,iters); 
       }
       vectimg *c = clone_vectimg(v); 
       change_image(c);
       
       if (!c->modified) { 
           free(c);
           continue;
       }
     
       rasterize_vectimg(c, buffer);

       long fitness = calc_fitness(buffer, ref);
       if (fitness < prev_fitness || prev_fitness < 0) {
          prev_fitness = fitness;
          free(v);
          v = c;       
          glfwSwapBuffers();
          selected++;
          
       }
       else {
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
