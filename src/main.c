#include <cglm/types.h>
#include <cglm/util.h>
#include <stdbool.h>
#include <stdlib.h>

#include "constants.h"
#include "engine.h"
#include "phongshader.h"

void processInput(GLFWwindow *window, Camera *c);
void draw(PhongShader shader, Model model, Camera c);

int main(void) {
  vec3 la = {0.5f, 0.5f, 0.5f};
  vec3 ld = {0.8f, 0.8f, 0.8f};
  vec3 ls = {1.0f, 1.0f, 1.0f};
  vec3 ldir = {-0.2f, -1.0f, -0.3f};

  GLFWwindow *window = app_init(SCR_WIDTH, SCR_HEIGHT);
  Material mat = gfx_material_create("assets/shader.vs", "assets/shader.fs", 2,
                                     "assets/container.png",
                                     "assets/container_specular.png");
  Mesh mesh = gfx_mesh_load(36, vertices, normals, textcoords);
  Camera c = {
      {0.0f, 0.0f, 5.0f}, {0, 0, 0},  {0.0f, 1.0f, 0.0},
      glm_rad(50.0f),     0.1,        100.0f,
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

  float angle = 0.0f;
  float step = 0.5f;
  while (app_running(window)) {
    processInput(window, &c);
    cube1.rotation[0] += step;
    cube1.rotation[1] += step / 2.0f;
    cube1.rotation[2] += step / 3.0f;

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
  mat4 p;
  mat4 v;
  mat4 m;

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
