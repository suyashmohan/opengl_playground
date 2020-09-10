#include "engine.h"

#include <mathc.h>
#include <stdbool.h>
#include <stdlib.h>

#include "constants.h"
#include "phongshader.h"

void processInput(GLFWwindow *window, Camera *c);
void draw(PhongShader shader, Model model, Camera c);

int main(void) {
  float la[VEC3_SIZE] = {0.5f, 0.5f, 0.5f};
  float ld[VEC3_SIZE] = {0.8f, 0.8f, 0.8f};
  float ls[VEC3_SIZE] = {1.0f, 1.0f, 1.0f};
  float ldir[VEC3_SIZE] = {-0.2f, -1.0f, -0.3f};

  GLFWwindow *window = app_init(SCR_WIDTH, SCR_HEIGHT);
  Material mat = gfx_material_create("assets/shader.vs", "assets/shader.fs", 2,
                                     "assets/container.png",
                                     "assets/container_specular.png");
  Mesh mesh = gfx_mesh_load(36, vertices, normals, textcoords);
  Camera c = {
      {0.0f, 0.0f, 5.0f}, {0, 0, 0},  {0.0f, 1.0f, 0.0},
      to_radians(50.0f),  0.1,        100.0f,
      SCR_WIDTH,          SCR_HEIGHT,
  };
  PhongShader shader;
  shader.mat = mat;

  // Prepare Shader
  phongshader_fetch_locations(&shader);
  phongshader_set_material(&shader, 0, 1, 32.0f);
  phongshader_set_light(&shader, la, ld, ls, ldir);

  Model cube1 = {mesh, {0.0f, 0.0f, 0.0f}, {50.0f, 30.0f, 0.0f}};
  Model cube2 = {mesh, {2.0f, 0.0f, -1.0f}, {0.0f, 50.0f, 0.0f}};
  Model cube3 = {mesh, {1.0f, -2.0f, 1.0f}, {0.0f, 0.0f, 50.0f}};
  Model cube4 = {mesh, {-2.0f, 1.0f, -2.0f}, {0.0f, 50.0f, 50.0f}};

  while (app_running(window)) {
    processInput(window, &c);

    // Clear the Screen
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render Begin
    draw(shader, cube1, c);
    draw(shader, cube2, c);
    draw(shader, cube3, c);
    draw(shader, cube4, c);
    // Render End

    app_swap_and_poll(window);
  }

  gfx_material_destroy(mat);
  gfx_mesh_free(mesh);
  app_quit(window);

  return EXIT_SUCCESS;
}

void draw(PhongShader shader, Model model, Camera c) {
  float p[MAT4_SIZE];
  float v[MAT4_SIZE];
  float m[MAT4_SIZE];

  gfx_camera_vp(v, p, c);
  gfx_model_mat4(m, model);

  gfx_material_use(shader.mat);
  phongshader_set_pvm(&shader, p, v, m, c.position);
  gfx_model_draw(model);
}

void processInput(GLFWwindow *window, Camera *c) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  } else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
    if (c->position[2] > 0) {
      c->position[2] -= 0.05;
    }
  } else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
    c->position[2] += 0.05;
  }
}
