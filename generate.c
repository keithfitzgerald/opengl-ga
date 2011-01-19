#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "GL/glfw.h"
#include "display.h"
#include "population.h"
#include "util.h"

void setup_display() {
    int vPort[4];

    if(!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        exit(EXIT_FAILURE);
    }

    if(!glfwOpenWindow(R_WIDTH,R_HEIGHT, 0, 0, 0, 0, 0, 0, GLFW_WINDOW)) {
        fprintf( stderr, "Failed to open GLFW window\n" );
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetWindowTitle("OpenGL");

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glDisable(GL_DEPTH_TEST);

    glGetIntegerv(GL_VIEWPORT, vPort);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    glOrtho(0, vPort[2], 0, vPort[3], -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
}

void print_vectimg(char *name, vectimg *v) {
   printf("vectimg[%s]: %d polygons\n",name,v->num_polygons);
   int sz = v->num_polygons; 
   for (int i = 0; i < sz;i++) {
       printf("    polygon[%d]: %d vertices\n",i,v->polygons[i].num_vertices);
   }
}

vectimg *clone_vectimg(vectimg *v) {
    vectimg *c = (vectimg*)malloc(sizeof(vectimg));
    int sz = v->num_polygons; 
    for (int i = 0; i < sz;i++) {
        c->polygons[i] = v->polygons[i];
    }
    c->num_polygons = v->num_polygons;
    c->modified = 0;

    return c;
}

int main(int argc, char **argv) {
 
    srand((unsigned int)time(0));
    setup_display();

    display_jpeg("firefox.jpg");
   
    byte *ref = (byte*)malloc(sizeof(byte) * 3 * 500 * 500);
    read_pixels(ref);
  
    long prev_fitness = -1;
    int selected = 0;
    vectimg *v = gen_random_vectimg();
 
    byte *buffer = (byte*)malloc(sizeof(byte) * 3 * (R_HEIGHT+1) * R_WIDTH);
    
    int i = 0, iters = 0;
    while(1) {
       iters++;
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

       printf("generation: %d selected: %d fitness: %ld polygons: %d iters: %d\r",i,selected,prev_fitness,v->num_polygons,iters);
       glfwPollEvents();
 
       if (glfwGetKey(GLFW_KEY_ESC) == GLFW_PRESS) {
          break;
       }
       i++;
    }    
    printf("\n");
 
    free(v);
    free(buffer);
    free(ref);
    glfwTerminate();

    exit(EXIT_SUCCESS);
}
