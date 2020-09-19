#include "engine.h"

typedef struct Light {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  vec3 direction;
} Light;

typedef struct PhongShader {
  Material mat;
  struct {
    int material_diffuse;
    int material_specular;
    int material_shininess;
    int light_ambient;
    int light_diffuse;
    int light_specular;
    int light_direction;
    int view;
    int projection;
    int model;
    int viewPos;
  } location;
} PhongShader;

PhongShader phong_create(Material mat);
void phong_set_material(PhongShader *shader, int diffuseTex, int specularTex,
                        float shininess);
void phong_set_light(PhongShader *shader, Light light);
void phong_set_pvm(PhongShader *shader, mat4 p, mat4 v, mat4 m, vec3 pos);
