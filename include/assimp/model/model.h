#ifndef model_H
#define model_H
#include "../../shaders/shader.h"
#include "../MeshType/dynMesh.h"
#include <assimp/cimport.h> // Plain-C interface
#include <assimp/defs.h>
#include <assimp/postprocess.h> // Post processing flags
#include <assimp/scene.h>       // Output data structure
#include <cglm/vec3.h>

typedef struct Model {
  char *directory;
  char *filePath;
  dynMesh meshes;
  Shader modelShader;
} Model;

void setupModel(Model *model, char *filePath, char *directory,
                Shader Modelshader);
void draw(Model *model, Shader *shader);
void loadModel(Model *model);
void sceneProcesssing(Model *model, const struct aiScene *scene);
void processNode(Model *model, const struct aiNode *node,
                 const struct aiScene *scene);
Mesh processMesh(Model *model, const struct aiMesh *meshPtr,
                 const struct aiScene *scene);
unsigned int textureFromFile(const char *path, const char *directory);

#endif
