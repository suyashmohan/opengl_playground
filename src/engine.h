#ifndef ENGINE_H
#define ENGINE_H

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

typedef struct Mesh {
    unsigned int vao;
    unsigned int ebo;
    unsigned int vbo_vertices;
    unsigned int vbo_colors;
    unsigned int vbo_texture;
} Mesh;

GLFWwindow* app_init();
void app_quit(GLFWwindow* window);
int app_running(GLFWwindow* window);
void app_swap_and_poll(GLFWwindow* window);

int gfx_shader_compile(GLenum type, const char* source);
int gfx_shader_program(const char* vertSrc, const char* fragSrc);
void gfx_shader_destroy(int shaderProgram);

unsigned int gfx_texture_load(const char* file);
void gfx_texture_destroy(unsigned int texture);

Mesh gfx_mesh_load(int countVertices, int countIndices, const float vertices[], const unsigned int indices[], const float colors[], const float textures[]);
void gfx_mesh_free(Mesh m);

#endif
