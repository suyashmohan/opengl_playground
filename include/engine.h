#ifndef ENGINE_H
#define ENGINE_H

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <cglm/cglm.h>

typedef struct Mesh {
  unsigned int vao;
  unsigned int vbo_vertices;
  unsigned int vbo_normals;
  unsigned int vbo_texture;
  int verticesCount;
} Mesh;

typedef struct Model {
  Mesh mesh;
  vec3 position;
  vec3 rotation;
} Model;

typedef struct Material {
  int shader;
  int textureCount;
  int *textures;
} Material;

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

GLFWwindow *app_init();
void app_quit(GLFWwindow *window);
int app_running(GLFWwindow *window);
void app_swap_and_poll(GLFWwindow *window);
char *app_readfile(const char *filePath);

int gfx_shader_compile(GLenum type, const char *source);
int gfx_shader_program(const char *vertSrc, const char *fragSrc);
void gfx_shader_destroy(int shaderProgram);

unsigned int gfx_texture_load(const char *file);
void gfx_texture_destroy(unsigned int texture);

Mesh gfx_mesh_load(int countVertices, const float vertices[],
                   const float normals[], const float textures[]);
void gfx_mesh_free(Mesh m);

Material gfx_material_create(const char *vrtSrcPath, const char *fragSrcPath,
                             int textureCount, const char *textures, ...);
void gfx_material_destroy(Material mat);
void gfx_material_use(Material mat);

void gfx_model_mat4(mat4 m, Model model);
void gfx_model_draw(Model model);

void gfx_camera_vp(mat4 v, mat4 p, Camera c);

#endif
