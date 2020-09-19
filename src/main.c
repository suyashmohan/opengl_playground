#include <cglm/types.h>
#include <cglm/util.h>
#include <stdbool.h>
#include <stdlib.h>

#include "engine.h"
#include "phongshader.h"

const int SCR_WIDTH = 1024;
const int SCR_HEIGHT = 600;

void processInput(GLFWwindow *window, Camera *c);
void draw(PhongShader *shader, Model model, Camera c);

int main(void) {
  GLFWwindow *window = app_init(SCR_WIDTH, SCR_HEIGHT);

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

  int shader = shader_create("assets/shader.vs", "assets/shader.fs");
  PhongShader phong = phong_create(shader);
  Geometry objGeo = geometry_load_obj("assets/cube.obj");
  Model cube1 = {objGeo, {-1.5f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}};
  phong_material_tex(&phong, 32.0f, "assets/container.png",
                     "assets/container_specular.png");
  phong_light(&phong, sun);

  int shader2 = shader_create("assets/shader.vs", "assets/shader.fs");
  PhongShader phong2 = phong_create(shader2);
  Geometry objGeo2 = geometry_load_obj("assets/suzanne.obj");
  Model cube2 = {objGeo2, {1.5f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}};
  vec3 diffuse = {0.0f, 0.5f, 0.31f};
  vec3 specular = {1.0f, 1.0f, 1.0f};
  phong_material(&phong2, diffuse, specular, 32.0f);
  phong_light(&phong2, sun);

  float angle = 0.0f;
  float step = 1.0f;
  vec4 bgColor = {0.2f, 0.3f, 0.3f, 1.0f};
  while (app_running(window, bgColor)) {
    processInput(window, &c);
    cube1.rotation[0] += step;
    cube1.rotation[1] += step / 2.0f;
    cube1.rotation[2] += step / 3.0f;

    cube2.rotation[1] += step / 3.0f;

    // Render Begin
    draw(&phong, cube1, c);
    draw(&phong2, cube2, c);
    // Render End

    app_swap_and_poll(window);
  }

  phong_destroy(&phong);
  geometry_free(objGeo);
  shader_destroy(shader);

  phong_destroy(&phong2);
  geometry_free(objGeo2);
  shader_destroy(shader2);

  app_quit(window);

  return EXIT_SUCCESS;
}

void draw(PhongShader *phong, Model model, Camera c) {
  mat4 p, v, m;

  camera_vp(v, p, c);
  model_mat4(m, model);

  phong_use(phong);
  phong_pvm(phong, p, v, m, c.position);
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
