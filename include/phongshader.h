#include "engine.h"

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

void phongshader_fetch_locations(PhongShader *shader);
void phongshader_set_material(PhongShader *shader, int diffuseTex,
                              int specularTex, float shininess);
void phongshader_set_light(PhongShader *shader, float *ambient, float *diffuse,
                           float *specular, float *direction);
void phongshader_set_pvm(PhongShader *shader, float *p, float *v, float *m,
                         float *pos);
