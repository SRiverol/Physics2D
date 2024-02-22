#ifndef model_H
#define model_H
#include "../MeshType/dynMesh.h"
#include "../../shaders/shader.h"
#include <cglm/vec3.h>

typedef struct Model {
  char* filePath;
  dynMesh meshes;
  Shader modelShader;
} Model;


void setupModel(Model* model, char* filePath, dynMesh meshes, Shader Modelshader);
void draw(Model* model);

#endif
