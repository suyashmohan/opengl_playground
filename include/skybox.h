#ifndef SKYBOX_H
#define SKYBOX_H

#include "engine.h"

typedef struct SkyBox {
  int shader;
  unsigned int cubemap;
  unsigned int vao;
  unsigned int vbo;
  struct {
    int skybox;
    int view;
    int projection;
  } location;
} SkyBox;

SkyBox skybox_init(unsigned int shader, unsigned int cubemap);
void skybox_draw(SkyBox skybox, mat4 view, mat4 projection);

#endif
