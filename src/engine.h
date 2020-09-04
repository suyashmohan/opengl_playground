#ifndef ENGINE_H
#define ENGINE_H

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <linmath.h>

typedef struct Mesh {
    unsigned int vao;
    unsigned int ebo;
    unsigned int vbo_vertices;
    unsigned int vbo_colors;
    unsigned int vbo_texture;
} Mesh;

typedef struct Camera {
    vec3 eye;
    vec3 lookat;
    vec3 up;
    float fov;
    float near;
    float far;
    float aspectRatio;
} Camera;

GLFWwindow* app_init();
void app_quit(GLFWwindow* window);
int app_running(GLFWwindow* window);
void app_swap_and_poll(GLFWwindow* window);

int gfx_shader_compile(GLenum type, const char* source);
int gfx_shader_program(const char* vertSrc, const char* fragSrc);
void gfx_shader_destroy(int shaderProgram);
void gfx_shader_uniform1i(int shaderProgram, const char* name, int val);
int gfx_shader_uniform_location(int shaderProgram, const char* name);

unsigned int gfx_texture_load(const char* file);
void gfx_texture_destroy(unsigned int texture);

Mesh gfx_mesh_load(int countVertices, int countIndices, const float vertices[], const unsigned int indices[], const float colors[], const float textures[]);
void gfx_mesh_free(Mesh m);

void gfx_camera_vp(mat4x4 vp, Camera c);

void print_mat4x4(mat4x4 m);

#endif
