#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "GL/glfw.h"
#include "pixbuf.h"
#include "display.h"
#include "population.h"

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

int cmp_by_fitness(const void *a, const void *b) 
{ 
    return ((citizen*)a)->fitness - ((citizen*)b)->fitness;
} 

int main(int argc, char **argv) {

    int i;
    pixbuf *ref = (pixbuf*)malloc(sizeof(pixbuf));
    generation *gen;

    srand(time(0));
    setup_display();

    display_jpeg("t4color.jpg");
    read_pixel_buffer(ref);
    
    //glfwSwapBuffers();  // display image to screen

    gen = create_first_generation(100, ref);
    qsort(&gen->citizens,100,sizeof(citizen),cmp_by_fitness);

    for (i = 0;i < 100;i++) {
        printf("citizen %d: %f\n",i,gen->citizens[i].fitness);
    }

    render_vectimg(gen->citizens[0].image);
    glfwSwapBuffers();

    free_generation(gen);

    free(ref);
    glfwTerminate();

    exit(EXIT_SUCCESS);
}
