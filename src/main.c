#include <cglm/types.h>
#include <cglm/util.h>
#include <stdbool.h>
#include <stdlib.h>

#include "engine.h"
#include "phongshader.h"
#define FAST_OBJ_IMPLEMENTATION
#include "fast_obj.h"

const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;

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

  fastObjMesh *objMesh = fast_obj_read("assets/cube.obj");
  if (objMesh == NULL) {
    printf("Unable to load OBJ file\n");
    exit(EXIT_FAILURE);
  }
  float *vertices = malloc(sizeof(float) * 3 * 3 * objMesh->face_count);
  float *texcoords = malloc(sizeof(float) * 2 * 3 * objMesh->face_count);
  float *normals = malloc(sizeof(float) * 3 * 3 * objMesh->face_count);
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
          vertices[vi] = objMesh->positions[3 * mi.p + 0];
          vertices[vi + 1] = objMesh->positions[3 * mi.p + 1];
          vertices[vi + 2] = objMesh->positions[3 * mi.p + 2];
          vi += 3;
        }

        if (mi.t) {
          texcoords[vti] = objMesh->texcoords[2 * mi.t + 0];
          texcoords[vti + 1] = objMesh->texcoords[2 * mi.t + 1];
          vti += 2;
        }

        if (mi.n) {
          normals[vni] = objMesh->normals[3 * mi.n + 0];
          normals[vni + 1] = objMesh->normals[3 * mi.n + 1];
          normals[vni + 2] = objMesh->normals[3 * mi.n + 2];
          vni += 3;
        }

        ++idx;
      }
    }
  }

  Mesh mesh =
      gfx_mesh_load(3 * objMesh->face_count, vertices, normals, texcoords);
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

  Model cube1 = {mesh, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}};
  // Model cube2 = {mesh, {2.0f, 0.0f, -1.0f}, {0.0f, 50.0f, 0.0f}};
  // Model cube3 = {mesh, {1.0f, -2.0f, 1.0f}, {0.0f, 0.0f, 50.0f}};
  // Model cube4 = {mesh, {-2.0f, 1.0f, -2.0f}, {0.0f, 50.0f, 50.0f}};

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
    // draw(shader, cube2, c);
    // draw(shader, cube3, c);
    // draw(shader, cube4, c);
    // Render End

    app_swap_and_poll(window);
  }

  fast_obj_destroy(objMesh);
  free(vertices);
  free(normals);
  free(texcoords);
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
