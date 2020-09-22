#ifndef ENGINE_H
#define ENGINE_H

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <glad/glad.h>

typedef struct Geometry {
  unsigned int vao;
  unsigned int vbo_vertices;
  unsigned int vbo_normals;
  unsigned int vbo_texture;
  int faceCount;
  float *vertices;
  float *normals;
  float *texcoords;
} Geometry;

typedef struct Transform {
  vec3 position;
  vec3 rotation;
  vec3 scale;
} Transform;

typedef struct Camera {
  vec3 position;
  vec3 target;
  vec3 up;
  float fov;
  float near;
  float far;
  float width;
  float height;
} Camera;

GLFWwindow *app_init(int width, int height);
void app_quit(GLFWwindow *window);
int app_running(GLFWwindow *window, vec4 color);
void app_swap_and_poll(GLFWwindow *window);
char *app_readfile(const char *filePath);

int shader_compile(GLenum type, const char *source);
int shader_program(const char *vertSrc, const char *fragSrc);
void shader_destroy(int shaderProgram);
int shader_create(const char *vrtSrcPath, const char *fragSrcPath);
void shader_use(int shader);

Geometry geometry_load_obj(const char *filePath);
void geometry_free(Geometry geo);
void geometry_draw(Geometry geo);

unsigned int texture_load(const char *file);
void texture_destroy(unsigned int texture);

unsigned int cubemap_load(int count, const char *file[]);
void cubemap_destroy(unsigned int cubemap);

void transform_mat4(mat4 m, Transform transform);
void camera_vp(mat4 v, mat4 p, Camera c);

#endif
