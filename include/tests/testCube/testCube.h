#ifndef testCube_H
#define testCube_H
#include "../../shaders/shader.h"
#include <cglm/vec3.h>

unsigned int setupCube();
void drawCube(unsigned int VAO, vec3 pos, float scale, Shader cubeShader);

#endif
