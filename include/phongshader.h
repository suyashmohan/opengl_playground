#include "engine.h"

typedef struct Light {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  vec3 direction;
} Light;

typedef struct PhongShader {
  int shader;
  struct {
    int material_diffuse;
    int material_specular;
    int material_shininess;
    int material_diffuse_clr;
    int material_specular_clr;
    int material_usediffusetex;
    int material_usespeculartex;
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

PhongShader phong_create(int shader);
void phong_material(PhongShader *phong, float shininess, vec3 difuse,
                    vec3 specular);
void phong_material_tex(PhongShader *phong, float shininess, int glDiffuseTex,
                        int glSpecularTex, vec3 specular);
void phong_light(PhongShader *phong, Light light);
void phong_pvm(PhongShader *phong, mat4 p, mat4 v, mat4 m, vec3 pos);
void phong_use(PhongShader *phong);