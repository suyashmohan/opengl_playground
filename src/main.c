#include <cglm/types.h>
#include <cglm/util.h>
#include <stdbool.h>
#include <stdlib.h>

#include "engine.h"
#include "phongshader.h"

const int SCR_WIDTH = 1024;
const int SCR_HEIGHT = 600;

void processInput(GLFWwindow *window, Camera *c);
void draw(PhongShader shader, Model model, Camera c);

int main(void) {
  GLFWwindow *window = app_init(SCR_WIDTH, SCR_HEIGHT);

  Geometry objGeo = geometry_load_obj("assets/cube.obj");
  Material mat = material_create("assets/shader.vs", "assets/shader.fs");
  material_textures(&mat, 2, "assets/container.png",
                    "assets/container_specular.png");
  PhongShader shader = phong_create(mat);

  Camera c = {
      {0.0f, 0.0f, 5.0f},
      {0.0f, 0.0f, 0.0f},
      {0.0f, 1.0f, 0.0f},
      glm_rad(50.0f),
      0.1f,
      100.0f,
      (float)SCR_WIDTH,
      (float)SCR_HEIGHT,
  };
  Light sun = {{0.5f, 0.5f, 0.5f},
               {0.8f, 0.8f, 0.8f},
               {1.0f, 1.0f, 1.0f},
               {-0.2f, -1.0f, -0.3f}};
  Model cube1 = {objGeo, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}};

  phong_set_material(&shader, 0, 1, 32.0f);
  phong_set_light(&shader, sun);

  float angle = 0.0f;
  float step = 0.5f;
  vec4 bgColor = {0.2f, 0.3f, 0.3f, 1.0f};
  while (app_running(window, bgColor)) {
    processInput(window, &c);
    cube1.rotation[0] += step;
    cube1.rotation[1] += step / 2.0f;
    cube1.rotation[2] += step / 3.0f;

    // Render Begin
    draw(shader, cube1, c);
    // Render End

    app_swap_and_poll(window);
  }

  material_destroy(mat);
  geometry_free(objGeo);
  app_quit(window);

  return EXIT_SUCCESS;
}

void draw(PhongShader shader, Model model, Camera c) {
  mat4 p;
  mat4 v;
  mat4 m;

  camera_vp(v, p, c);
  model_mat4(m, model);

  material_use(shader.mat);
  phong_set_pvm(&shader, p, v, m, c.position);
  model_draw(model);
}

void processInput(GLFWwindow *window, Camera *c) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }

  if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
      if (c->position[2] > 0) {
        c->position[2] -= 0.05f;
      }
    } else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
      c->position[2] += 0.05f;
    }
  }
}
