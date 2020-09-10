#include "engine.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

GLFWwindow *app_init(int width, int height) {
  if (!glfwInit()) {
    printf("Unable to init glfw");
    exit(EXIT_FAILURE);
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

  GLFWwindow *window = glfwCreateWindow(width, height, "OpenGL", NULL, NULL);
  if (window == NULL) {
    printf("Unable to create window");
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    printf("Unabel to load OpenGL");
    exit(EXIT_FAILURE);
  }
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  printf("OpenGL Version: %s\n", glGetString(GL_VERSION));

  return window;
}

void app_quit(GLFWwindow *window) {
  glfwDestroyWindow(window);
  glfwTerminate();
}

int app_running(GLFWwindow *window) { return !glfwWindowShouldClose(window); }

void app_swap_and_poll(GLFWwindow *window) {
  glfwSwapBuffers(window);
  glfwPollEvents();
}

char *app_readfile(const char *filePath) {
  FILE *fp;
  long size;
  char *buffer;
  size_t result;

  fp = fopen(filePath, "r");
  if (fp == NULL) {
    printf("Error opening file: %s", filePath);
    return NULL;
  }

  fseek(fp, 0, SEEK_END);
  size = ftell(fp);
  rewind(fp);

  buffer = (char *)malloc(sizeof(char) * (size + 1));
  result = fread(buffer, sizeof(char), size, fp);
  buffer[result] = '\0';

  fclose(fp);
  return buffer;
}

int gfx_shader_compile(GLenum type, const char *source) {
  int success;
  char infoLog[512];

  int shader = glCreateShader(type);
  glShaderSource(shader, 1, &source, NULL);
  glCompileShader(shader);
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
    return 0;
  }
  return shader;
}

int gfx_shader_program(const char *vertSrc, const char *fragSrc) {
  int success;
  char infoLog[512];

  int vertexShader = gfx_shader_compile(GL_VERTEX_SHADER, vertSrc);
  int fragmentShader = gfx_shader_compile(GL_FRAGMENT_SHADER, fragSrc);

  if (!vertexShader || !fragmentShader) {
    return 0;
  }

  int shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", infoLog);
    return 0;
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return shaderProgram;
}

void gfx_shader_destroy(int shaderProgram) { glDeleteProgram(shaderProgram); }

unsigned int gfx_texture_load(const char *file) {
  // load texture
  unsigned int texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  int imgW, imgH, nrChannels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char *data = stbi_load(file, &imgW, &imgH, &nrChannels, 0);
  if (!data) {
    printf("Unabel to load image: %s\n", file);
    return 0;
  }
  printf("Image Loaded : %s - %dx%d\n", file, imgW, imgH);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgW, imgH, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(data);

  return texture;
}

void gfx_texture_destroy(unsigned int texture) {
  glDeleteTextures(1, &texture);
}

Mesh gfx_mesh_load(int countVertices, const float vertices[],
                   const float normals[], const float textures[]) {
  Mesh m;
  m.verticesCount = countVertices;

  glGenVertexArrays(1, &m.vao);
  glBindVertexArray(m.vao);

  // position attribute
  glGenBuffers(1, &m.vbo_vertices);
  glBindBuffer(GL_ARRAY_BUFFER, m.vbo_vertices);
  glBufferData(GL_ARRAY_BUFFER, countVertices * 3 * sizeof(float), vertices,
               GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);

  // normal attribute
  glGenBuffers(1, &m.vbo_normals);
  glBindBuffer(GL_ARRAY_BUFFER, m.vbo_normals);
  glBufferData(GL_ARRAY_BUFFER, countVertices * 3 * sizeof(float), normals,
               GL_STATIC_DRAW);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(1);

  // texture attribute
  glGenBuffers(1, &m.vbo_texture);
  glBindBuffer(GL_ARRAY_BUFFER, m.vbo_texture);
  glBufferData(GL_ARRAY_BUFFER, countVertices * 2 * sizeof(float), textures,
               GL_STATIC_DRAW);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(2);

  return m;
}

void gfx_mesh_free(Mesh m) {
  glDeleteVertexArrays(1, &m.vao);
  glDeleteBuffers(1, &m.vbo_normals);
  glDeleteBuffers(1, &m.vbo_texture);
  glDeleteBuffers(1, &m.vbo_vertices);
}

Material gfx_material_create(const char *vrtSrcPath, const char *fragSrcPath,
                             int textureCount, const char *textures, ...) {
  Material mat;

  char *vrtSrc = app_readfile(vrtSrcPath);
  char *fragSrc = app_readfile(fragSrcPath);
  mat.shader = gfx_shader_program(vrtSrc, fragSrc);
  free(vrtSrc);
  free(fragSrc);

  if (mat.shader == 0) {
    exit(EXIT_FAILURE);
  }

  mat.textureCount = textureCount;
  mat.textures = NULL;
  if (textureCount > 0) {
    mat.textures = malloc(sizeof(int));
    va_list args;
    va_start(args, textures);
    for (int i = 0; i < mat.textureCount; i++) {
      unsigned int tex = gfx_texture_load(textures);
      if (tex == 0) {
        exit(EXIT_FAILURE);
      }
      mat.textures[i] = tex;
      textures = va_arg(args, const char *);
    }
    va_end(args);
  }
  return mat;
}

void gfx_material_destroy(Material mat) {
  gfx_shader_destroy(mat.shader);
  if (mat.textureCount > 0) {
    for (int i = 0; i < mat.textureCount; ++i) {
      gfx_texture_destroy(mat.textures[i]);
    }
  }
}

void gfx_material_use(Material mat) {
  glUseProgram(mat.shader);
  for (int i = 0; i < mat.textureCount; i++) {
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, mat.textures[i]);
  }
}

void gfx_model_mat4(float *m, Model model) {
  mat4_identity(m);
  mat4_rotation_x(m, to_radians(model.rotation[0]));
  mat4_rotation_y(m, to_radians(model.rotation[1]));
  mat4_rotation_z(m, to_radians(model.rotation[2]));
  mat4_translate(m, m, model.position);
}

void gfx_model_draw(Model model) {
  glBindVertexArray(model.mesh.vao);
  glDrawArrays(GL_TRIANGLES, 0, model.mesh.verticesCount);
}

void gfx_camera_vp(float *v, float *p, Camera c) {
  mat4_look_at(v, c.position, c.target, c.up);
  mat4_perspective_fov(p, c.fov, c.width, c.height, c.near, c.far);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}
