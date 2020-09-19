#include "phongshader.h"
#include <stdarg.h>

void material_fetch_locations(PhongShader *phong);

PhongShader phong_create(int shader) {
  PhongShader phong;
  phong.shader = shader;
  material_fetch_locations(&phong);
  phong_use(&phong);
  return phong;
}

void phong_material_tex(PhongShader *phong, float shininess, int glDiffuseTex,
                        int glSpecularTex, vec3 specular) {
  glUniform1f(phong->location.material_shininess, shininess);
  glUniform1i(phong->location.material_usediffusetex, 1);
  glUniform1i(phong->location.material_diffuse, 0);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, glDiffuseTex);

  if (glSpecularTex > 0) {
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, glSpecularTex);
    glUniform1i(phong->location.material_usespeculartex, 1);
    glUniform1i(phong->location.material_specular, 1);
  } else {
    glUniform1i(phong->location.material_usespeculartex, 0);
    if (specular == NULL) {
      vec3 specularDefault = {0.5f, 0.5f, 0.5f};
      glUniform3fv(phong->location.material_specular_clr, 1,
                   (const GLfloat *)specularDefault);
    } else {
      glUniform3fv(phong->location.material_specular_clr, 1,
                   (const GLfloat *)specular);
    }
  }
}

void phong_material(PhongShader *phong, float shininess, vec3 difuse,
                    vec3 specular) {
  glUniform1i(phong->location.material_usediffusetex, 0);
  glUniform1i(phong->location.material_usespeculartex, 0);
  glUniform1f(phong->location.material_shininess, shininess);
  glUniform3fv(phong->location.material_diffuse_clr, 1,
               (const GLfloat *)difuse);
  glUniform3fv(phong->location.material_specular_clr, 1,
               (const GLfloat *)specular);
}

void phong_light(PhongShader *phong, Light light) {
  glUniform3fv(phong->location.light_ambient, 1,
               (const GLfloat *)light.ambient);
  glUniform3fv(phong->location.light_diffuse, 1,
               (const GLfloat *)light.diffuse);
  glUniform3fv(phong->location.light_specular, 1,
               (const GLfloat *)light.specular);
  glUniform3fv(phong->location.light_direction, 1,
               (const GLfloat *)light.direction);
}

void phong_pvm(PhongShader *phong, mat4 p, mat4 v, mat4 m, vec3 pos) {
  glUniformMatrix4fv(phong->location.view, 1, GL_FALSE, (const GLfloat *)v);
  glUniformMatrix4fv(phong->location.projection, 1, GL_FALSE,
                     (const GLfloat *)p);
  glUniformMatrix4fv(phong->location.model, 1, GL_FALSE, (const GLfloat *)m);
  glUniform3fv(phong->location.viewPos, 1, (const GLfloat *)pos);
}

void phong_use(PhongShader *phong) { glUseProgram(phong->shader); }

void material_fetch_locations(PhongShader *phong) {
  int shader = phong->shader;
  phong->location.material_diffuse =
      glGetUniformLocation(shader, "material.diffuse");
  phong->location.material_specular =
      glGetUniformLocation(shader, "material.specular");
  phong->location.material_shininess =
      glGetUniformLocation(shader, "material.shininess");
  phong->location.material_diffuse_clr =
      glGetUniformLocation(shader, "material.diffuseColor");
  phong->location.material_specular_clr =
      glGetUniformLocation(shader, "material.specularColor");
  phong->location.material_usediffusetex =
      glGetUniformLocation(shader, "material.useDiffuseTexture");
  phong->location.material_usespeculartex =
      glGetUniformLocation(shader, "material.useSpecularTexture");
  phong->location.light_ambient = glGetUniformLocation(shader, "light.ambient");
  phong->location.light_diffuse = glGetUniformLocation(shader, "light.diffuse");
  phong->location.light_specular =
      glGetUniformLocation(shader, "light.specular");
  phong->location.light_direction =
      glGetUniformLocation(shader, "light.direction");
  phong->location.view = glGetUniformLocation(shader, "view");
  phong->location.projection = glGetUniformLocation(shader, "projection");
  phong->location.model = glGetUniformLocation(shader, "model");
  phong->location.viewPos = glGetUniformLocation(shader, "viewPos");
}