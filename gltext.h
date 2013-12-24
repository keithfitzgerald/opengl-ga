#ifndef __GLTEXT_H__
#define __GLTEXT_H__

#include <glfw/glfw3.h>

typedef struct {
    GLuint program;
    GLuint position;
    GLuint normal;
    GLuint uv;
    GLuint matrix;
    GLuint sampler;
    GLuint camera;
    GLuint timer;
    GLuint extra1;
    GLuint extra2;
} Attrib;

void del_buffer(GLuint buffer);
GLfloat *malloc_faces(int components, int faces);
void make_character(float *data, float x, float y, float n, float m, char c);
GLuint gen_text_buffer(float x, float y, float n, char *text);
void draw_triangles_2d(Attrib *attrib, GLuint buffer, int count);
void draw_text(Attrib *attrib, GLuint buffer, int length);
void render_text(Attrib *attrib, int justify, float x, float y, float n, char *text, int height, int width);
char *load_file(const char *path);
GLuint make_shader(GLenum type, const char *source);
GLuint load_shader(GLenum type, const char *path);
GLuint make_program(GLuint shader1, GLuint shader2);
GLuint load_program(const char *path1, const char *path2);
void load_png_texture(const char *file_name);

#endif /* __GLTEXT_H__ */
