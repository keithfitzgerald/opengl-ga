#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "GL/glfw.h"
#include "polygon.h"
#include "bitmap.h"
#include "util.h"

void render_image(polygon image[]) {
    int i,j;
    for (i = 0; i < MAX_POLYGONS;i++) {
        polygon *p = &image[i];

        glBegin(GL_POLYGON);
        glColor4f(p->color.r, p->color.g, p->color.b, p->color.alpha);

        for (j = 0;j < p->num_vertices;j++) {
            vertex v = p->vertices[j];
            //printf("draw vertex %d: x=%f,y=%f\n",j,v.x,v.y);
            glVertex3f(v.x, v.y, 0.0f);
        }

        glEnd();
    }
}

void setup_display(void) {
    int width, height;
    glfwGetWindowSize(&width, &height);

    // TODO: do these need to be called everytime?
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glDisable(GL_DEPTH_TEST);

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    glOrtho (0, width, height, 0, 0, 1);
    glMatrixMode(GL_MODELVIEW);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); 
    glClear(GL_COLOR_BUFFER_BIT); 
    glLoadIdentity();
}

int main(int argc, char **argv) {
    polygon *image;
    bitmap *origImage = (bitmap*)malloc(sizeof(bitmap));

    readJPEG("c:/firefox.jpg", origImage);

    srand(time(0));

    if(!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        exit(EXIT_FAILURE);
    }

    if( !glfwOpenWindow(R_HEIGHT, R_WIDTH, 0, 0, 0, 0, 0, 0, GLFW_WINDOW)) {
        fprintf( stderr, "Failed to open GLFW window\n" );
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetWindowTitle("OpenGL");
    glfwEnable(GLFW_STICKY_KEYS);
    glfwSwapInterval(1);

    do {
        
        image = gen_random_image();
        render_image(image);
        free_image(image);
        
        glfwSwapBuffers();

    } while(glfwGetKey(GLFW_KEY_ESC) != GLFW_PRESS &&
            glfwGetWindowParam(GLFW_OPENED));

    free(origImage);
    glfwTerminate();

    exit(EXIT_SUCCESS);
}
