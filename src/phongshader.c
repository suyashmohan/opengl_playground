#include "phongshader.h"

void phongshader_fetch_locations(PhongShader *shader) {
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

void phongshader_set_material(PhongShader *shader, int diffuseTex,
                              int specularTex, float shininess) {
  glUseProgram(shader->mat.shader);
  Material mat = shader->mat;
  glUniform1i(shader->location.material_diffuse, 0);
  glUniform1i(shader->location.material_specular, 1);
  glUniform1f(shader->location.material_shininess, shininess);
}

void phongshader_set_light(PhongShader *shader, float *ambient, float *diffuse,
                           float *specular, float *direction) {
  glUseProgram(shader->mat.shader);
  glUniform3fv(shader->location.light_ambient, 1, (const GLfloat *)ambient);
  glUniform3fv(shader->location.light_diffuse, 1, (const GLfloat *)diffuse);
  glUniform3fv(shader->location.light_specular, 1, (const GLfloat *)specular);
  glUniform3fv(shader->location.light_direction, 1, (const GLfloat *)direction);
}

void phongshader_set_pvm(PhongShader *shader, float *p, float *v, float *m,
                         float *pos) {
  glUniformMatrix4fv(shader->location.view, 1, GL_FALSE, (const GLfloat *)v);
  glUniformMatrix4fv(shader->location.projection, 1, GL_FALSE,
                     (const GLfloat *)p);
  glUniformMatrix4fv(shader->location.model, 1, GL_FALSE, (const GLfloat *)m);
  glUniform3fv(shader->location.viewPos, 1, (const GLfloat *)pos);
}
