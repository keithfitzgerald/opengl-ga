#include <stdlib.h>
#include <stdio.h>
#include "GL/glfw.h"
#include "display.h"
#include "../gautil.h"
#include "../jpeg.h"

void setup_display(int width, int height) {
    int vPort[4];

    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        exit(EXIT_FAILURE);
    }

    if (!glfwOpenWindow(width, height, 0, 0, 0, 0, 0, 0, GLFW_WINDOW)) {
        fprintf(stderr, "Failed to open GLFW window\n");
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

void render_vectimg(vectimg *v) {
    for (int i = 0; i < v->num_polygons; i++) {
        polygon p = v->polygons[i];

        glBegin(GL_POLYGON);
        // TODO: why doesn't this work?
        //glColor4iv(p.color);

        glColor4ub(p.color[0], p.color[1], p.color[2], p.color[3]);

        for (int j = 0; j < p.num_vertices; j++) {
            vertex v = p.vertices[j];
            glVertex2i(v.x, v.y);
        }

        glEnd();
    }
}

void read_pixels(byte *buffer, int width, int height) {
    glReadPixels(0, 0, width - 1, height - 1, GL_RGB, GL_UNSIGNED_BYTE, buffer);
}

void rasterize_vectimg(vectimg *v, byte *buffer) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    render_vectimg(v);
    read_pixels(buffer, v->width, v->height);
}

/*
void display_rgb_pixbuf(byte *buffer, int width, int height, unsigned char inverted) {
    int stride = width * 3;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < stride; x += 3) {
            int i = (y * stride) + x;
            printf("out pixel [%d,%d]: R=%d,G=%d,B=%d\n",x/3,y,buffer[i],buffer[i+1],buffer[i+2]);
            fflush(stdout);
            glBegin(GL_POINTS);
            glColor3ub(buffer[i], buffer[i + 1], buffer[i + 2]);
            glVertex2i(x / 3, y);
            glEnd();
        }
    }
}
*/


void display_rgb_pixbuf(byte *buffer, int width, int height, unsigned char inverted) {
    int base = inverted ? height - 1 : 0;
    int stride = width * 3;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < stride; x += 3) {
            int i = ((base - y) * stride) + x;
            //printf("out pixel [%d,%d]: R=%d,G=%d,B=%d\n",x/3,y,buffer[i],buffer[i+1],buffer[i+2]);
            //fflush(stdout);
            glBegin(GL_POINTS);
            glColor3ub(buffer[i], buffer[i + 1], buffer[i + 2]);
            glVertex2i(x / 3, y);
            glEnd();
        }
    }
}

