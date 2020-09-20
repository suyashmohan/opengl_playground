#include <cglm/types.h>
#include <cglm/util.h>
#include <stdbool.h>
#include <stdlib.h>

#include "engine.h"
#include "phongshader.h"

const int SCR_WIDTH = 1024;
const int SCR_HEIGHT = 600;

void processInput(GLFWwindow *window, Camera *c);

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

  int texDiffuse = texture_load("assets/container.png");
  int texSpecular = texture_load("assets/container_specular.png");
  vec3 diffuse = {0.0f, 0.5f, 0.31f};
  vec3 specular = {1.0f, 1.0f, 1.0f};

  Geometry objGeo = geometry_load_obj("assets/cube.obj");
  Transform cube1 = {
      {-1.5f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}};

  Geometry objGeo2 = geometry_load_obj("assets/suzanne.obj");
  Transform cube2 = {
      {1.5f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}};

  float angle = 0.0f;
  float step = 1.0f;
  vec4 bgColor = {0.2f, 0.3f, 0.3f, 1.0f};
  mat4 p, v, m;
  double previousTime = glfwGetTime();
  int frameCount = 0;
  while (app_running(window, bgColor)) {
      double currentTime = glfwGetTime();
      frameCount++;
      if (currentTime - previousTime >= 1.0)
      {
          printf("FPS: %d\n", frameCount);
          frameCount = 0;
          previousTime = currentTime;
      }

    processInput(window, &c);
    cube1.rotation[0] += step;
    cube1.rotation[1] += step / 2.0f;
    cube1.rotation[2] += step / 3.0f;

    cube2.rotation[1] += step / 3.0f;

    camera_vp(v, p, c);

    // Render Begin
    phong_use(phong);
    phong_light(phong, sun);

    transform_mat4(m, cube1);
    phong_pvm(phong, p, v, m, c.position);
    phong_material_tex(phong, 32.0f, texDiffuse, texSpecular, NULL);
    geometry_draw(objGeo);

    transform_mat4(m, cube2);
    phong_pvm(phong, p, v, m, c.position);
    phong_material(phong, 32.0f, diffuse, specular);
    geometry_draw(objGeo2);
    // Render End

    app_swap_and_poll(window);
  }

  shader_destroy(shader);
  geometry_free(objGeo);
  geometry_free(objGeo2);
  texture_destroy(texDiffuse);
  texture_destroy(texSpecular);

  app_quit(window);

  return EXIT_SUCCESS;
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
