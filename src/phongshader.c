#include "phongshader.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <stdarg.h>

unsigned int texture_load(const char *file);
void texture_destroy(unsigned int texture);
void material_textures(PhongShader *phong, int textureCount,
                       const char *textures, ...);
void material_fetch_locations(PhongShader *phong);

PhongShader phong_create(int shader) {
  PhongShader phong;
  phong.shader = shader;
  phong.textureCount = 0;
  phong.textures = NULL;
  material_fetch_locations(&phong);
  phong_use(&phong);
  return phong;
}

void phong_material_tex(PhongShader *phong, float shininess,
                        const char *diffuseTex, const char *specularTex) {
  glUniform1f(phong->location.material_shininess, shininess);
  glUniform1i(phong->location.material_diffuse, 0);
  glUniform1i(phong->location.material_usediffusetex, 1);

  if (specularTex != NULL) {
    material_textures(phong, 2, diffuseTex, specularTex);
    glUniform1i(phong->location.material_specular, 1);
    glUniform1i(phong->location.material_usespeculartex, 1);
  } else {
    material_textures(phong, 1, diffuseTex);
    glUniform1i(phong->location.material_usespeculartex, 0);
  }
}

void phong_material(PhongShader *phong, vec3 color, vec3 specular,
                    float shininess) {
  glUniform1i(phong->location.material_usediffusetex, 0);
  glUniform1i(phong->location.material_usespeculartex, 0);
  glUniform1f(phong->location.material_shininess, shininess);
  glUniform3fv(phong->location.material_diffuse_clr, 1, (const GLfloat *)color);
  glUniform3fv(phong->location.material_diffuse_clr, 1, (const GLfloat *)color);
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

void phong_destroy(PhongShader *phong) {
  // shader_destroy(phong->shader);
  if (phong->textureCount > 0) {
    for (int i = 0; i < phong->textureCount; ++i) {
      texture_destroy(phong->textures[i]);
    }
  }
}

void phong_use(PhongShader *phong) {
  glUseProgram(phong->shader);
  for (int i = 0; i < phong->textureCount; i++) {
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, phong->textures[i]);
  }
}

unsigned int texture_load(const char *file) {
  // load texture
  unsigned int texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  int imgW, imgH, nrChannels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char *data = stbi_load(file, &imgW, &imgH, &nrChannels, 0);
  if (!data) {
    printf("Unabel to load image: %s\n", file);
    return 0;
  }
  printf("Image Loaded : %s - %dx%d [%d]\n", file, imgW, imgH, nrChannels);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgW, imgH, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(data);

  return texture;
}

void texture_destroy(unsigned int texture) { glDeleteTextures(1, &texture); }

void material_textures(PhongShader *phong, int textureCount,
                       const char *textures, ...) {
  phong->textureCount = textureCount;
  if (phong->textures != NULL) {
    printf("Textures already loaded for Material");
    exit(EXIT_FAILURE);
  }

  if (textureCount > 0) {
    phong->textures = malloc(sizeof(int));
    va_list args;
    va_start(args, textures);
    for (int i = 0; i < phong->textureCount; i++) {
      unsigned int tex = texture_load(textures);
      if (tex == 0) {
        exit(EXIT_FAILURE);
      }
      phong->textures[i] = tex;
      textures = va_arg(args, const char *);
    }
    va_end(args);
  }
}

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