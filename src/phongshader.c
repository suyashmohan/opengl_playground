#include "phongshader.h"

void phong_fetch_locations(PhongShader *shader);

PhongShader phong_create(Material mat) {
  PhongShader shader;
  shader.mat = mat;
  phong_fetch_locations(&shader);
  return shader;
}

void phong_fetch_locations(PhongShader *shader) {
  Material mat = shader->mat;
  shader->location.material_diffuse =
      glGetUniformLocation(mat.shader, "material.diffuse");
  shader->location.material_specular =
      glGetUniformLocation(mat.shader, "material.specular");
  shader->location.material_shininess =
      glGetUniformLocation(mat.shader, "material.shininess");
  shader->location.light_ambient =
      glGetUniformLocation(mat.shader, "light.ambient");
  shader->location.light_diffuse =
      glGetUniformLocation(mat.shader, "light.diffuse");
  shader->location.light_specular =
      glGetUniformLocation(mat.shader, "light.specular");
  shader->location.light_direction =
      glGetUniformLocation(mat.shader, "light.direction");
  shader->location.view = glGetUniformLocation(mat.shader, "view");
  shader->location.projection = glGetUniformLocation(mat.shader, "projection");
  shader->location.model = glGetUniformLocation(mat.shader, "model");
  shader->location.viewPos = glGetUniformLocation(mat.shader, "viewPos");
}

void phong_set_material(PhongShader *shader, int diffuseTex, int specularTex,
                        float shininess) {
  glUseProgram(shader->mat.shader);
  glUniform1i(shader->location.material_diffuse, 0);
  glUniform1i(shader->location.material_specular, 1);
  glUniform1f(shader->location.material_shininess, shininess);
}

void phong_set_light(PhongShader *shader, Light light) {
  glUseProgram(shader->mat.shader);
  glUniform3fv(shader->location.light_ambient, 1,
               (const GLfloat *)light.ambient);
  glUniform3fv(shader->location.light_diffuse, 1,
               (const GLfloat *)light.diffuse);
  glUniform3fv(shader->location.light_specular, 1,
               (const GLfloat *)light.specular);
  glUniform3fv(shader->location.light_direction, 1,
               (const GLfloat *)light.direction);
}

void phong_set_pvm(PhongShader *shader, mat4 p, mat4 v, mat4 m, vec3 pos) {
  glUniformMatrix4fv(shader->location.view, 1, GL_FALSE, (const GLfloat *)v);
  glUniformMatrix4fv(shader->location.projection, 1, GL_FALSE,
                     (const GLfloat *)p);
  glUniformMatrix4fv(shader->location.model, 1, GL_FALSE, (const GLfloat *)m);
  glUniform3fv(shader->location.viewPos, 1, (const GLfloat *)pos);
}
