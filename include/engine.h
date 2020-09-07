#ifndef ENGINE_H
#define ENGINE_H

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <mathc.h>

typedef struct Mesh {
    unsigned int vao;
    unsigned int vbo_vertices;
    unsigned int vbo_normals;
    unsigned int vbo_texture;
} Mesh;

typedef struct Camera {
    mfloat_t eye[VEC3_SIZE];
    mfloat_t lookat[VEC3_SIZE];
    mfloat_t up[VEC3_SIZE];
    float fov;
    float near;
    float far;
    float width;
    float height;
} Camera;

GLFWwindow* app_init();
void app_quit(GLFWwindow* window);
int app_running(GLFWwindow* window);
void app_swap_and_poll(GLFWwindow* window);
char* app_readfile(const char* filePath);

int gfx_shader_compile(GLenum type, const char* source);
int gfx_shader_program(const char* vertSrc, const char* fragSrc);
void gfx_shader_destroy(int shaderProgram);

unsigned int gfx_texture_load(const char* file);
void gfx_texture_destroy(unsigned int texture);

Mesh gfx_mesh_load(int countVertices, const float vertices[], const float normals[], const float textures[]);
void gfx_mesh_free(Mesh m);

void gfx_camera_vp(mfloat_t *vp, Camera c);

#endif
