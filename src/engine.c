#include "engine.h"
#include <cglm/affine.h>
#include <cglm/cam.h>
#include <cglm/mat4.h>
#include <cglm/util.h>
#define FAST_OBJ_IMPLEMENTATION
#include "fast_obj.h"

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
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_CULL_FACE);
  glFrontFace(GL_CCW);

  printf("OpenGL Version: %s\n", glGetString(GL_VERSION));

  return window;
}

void app_quit(GLFWwindow *window) {
  glfwDestroyWindow(window);
  glfwTerminate();
}

int app_running(GLFWwindow *window, vec4 color) {
  // Clear the Screen
  glClearColor(color[0], color[1], color[2], color[3]);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  return !glfwWindowShouldClose(window);
}

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

int shader_compile(GLenum type, const char *source) {
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

int shader_program(const char *vertSrc, const char *fragSrc) {
  int success;
  char infoLog[512];

  int vertexShader = shader_compile(GL_VERTEX_SHADER, vertSrc);
  int fragmentShader = shader_compile(GL_FRAGMENT_SHADER, fragSrc);

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

void shader_destroy(int shaderProgram) { glDeleteProgram(shaderProgram); }

Geometry geometry_load_obj(const char *filepath) {
  Geometry obj;

  fastObjMesh *objMesh = fast_obj_read(filepath);
  if (objMesh == NULL) {
    printf("Unable to load OBJ file: %s\n", filepath);
    exit(EXIT_FAILURE);
  }
  obj.faceCount = objMesh->face_count;
  obj.vertices = malloc(sizeof(float) * 3 * 3 * obj.faceCount);
  obj.texcoords = malloc(sizeof(float) * 2 * 3 * obj.faceCount);
  obj.normals = malloc(sizeof(float) * 3 * 3 * obj.faceCount);

  unsigned int vi, vni, vti;
  vi = vni = vti = 0;
  for (unsigned int ii = 0; ii < objMesh->group_count; ++ii) {
    const fastObjGroup grp = objMesh->groups[ii];
    int idx = 0;
    for (unsigned int jj = 0; jj < grp.face_count; ++jj) {
      unsigned int fv = objMesh->face_vertices[grp.face_offset + jj];
      for (unsigned int kk = 0; kk < fv; ++kk) {
        fastObjIndex mi = objMesh->indices[grp.index_offset + idx];
        if (mi.p) {
          obj.vertices[vi] = objMesh->positions[3 * mi.p + 0];
          obj.vertices[vi + 1] = objMesh->positions[3 * mi.p + 1];
          obj.vertices[vi + 2] = objMesh->positions[3 * mi.p + 2];
          vi += 3;
        }

        if (mi.t) {
          obj.texcoords[vti] = objMesh->texcoords[2 * mi.t + 0];
          obj.texcoords[vti + 1] = objMesh->texcoords[2 * mi.t + 1];
          vti += 2;
        }

        if (mi.n) {
          obj.normals[vni] = objMesh->normals[3 * mi.n + 0];
          obj.normals[vni + 1] = objMesh->normals[3 * mi.n + 1];
          obj.normals[vni + 2] = objMesh->normals[3 * mi.n + 2];
          vni += 3;
        }

        ++idx;
      }
    }
  }

  fast_obj_destroy(objMesh);

  glGenVertexArrays(1, &obj.vao);
  glBindVertexArray(obj.vao);

  // position attribute
  glGenBuffers(1, &obj.vbo_vertices);
  glBindBuffer(GL_ARRAY_BUFFER, obj.vbo_vertices);
  glBufferData(GL_ARRAY_BUFFER, obj.faceCount * 3 * 3 * sizeof(float),
               obj.vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);

  // normal attribute
  glGenBuffers(1, &obj.vbo_normals);
  glBindBuffer(GL_ARRAY_BUFFER, obj.vbo_normals);
  glBufferData(GL_ARRAY_BUFFER, obj.faceCount * 3 * 3 * sizeof(float),
               obj.normals, GL_STATIC_DRAW);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(1);

  // texture attribute
  glGenBuffers(1, &obj.vbo_texture);
  glBindBuffer(GL_ARRAY_BUFFER, obj.vbo_texture);
  glBufferData(GL_ARRAY_BUFFER, obj.faceCount * 3 * 2 * sizeof(float),
               obj.texcoords, GL_STATIC_DRAW);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(2);

  return obj;
}

void geometry_free(Geometry g) {
  free(g.vertices);
  free(g.texcoords);
  free(g.normals);
  glDeleteVertexArrays(1, &g.vao);
  glDeleteBuffers(1, &g.vbo_normals);
  glDeleteBuffers(1, &g.vbo_texture);
  glDeleteBuffers(1, &g.vbo_vertices);
}

int shader_create(const char *vrtSrcPath, const char *fragSrcPath) {
  int shader;

  char *vrtSrc = app_readfile(vrtSrcPath);
  char *fragSrc = app_readfile(fragSrcPath);
  shader = shader_program(vrtSrc, fragSrc);
  free(vrtSrc);
  free(fragSrc);

  if (shader == 0) {
    exit(EXIT_FAILURE);
  }

  return shader;
}

void shader_use(int shader) { glUseProgram(shader); }

void model_mat4(mat4 m, Model model) {
  glm_mat4_identity(m);
  glm_translate(m, model.position);
  glm_rotate_x(m, glm_rad(model.rotation[0]), m);
  glm_rotate_y(m, glm_rad(model.rotation[1]), m);
  glm_rotate_z(m, glm_rad(model.rotation[2]), m);
}

void model_draw(Model model) {
  glBindVertexArray(model.geometry.vao);
  glDrawArrays(GL_TRIANGLES, 0, model.geometry.faceCount * 3);
}

void camera_vp(mat4 v, mat4 p, Camera c) {
  glm_lookat(c.position, c.target, c.up, v);
  glm_perspective(c.fov, c.width / c.height, c.near, c.far, p);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}
