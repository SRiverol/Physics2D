#include <cglm/vec3.h>
#include "../IndexType/dynIndex.h"
#include "../TextureType/dynTexture.h"
#include "../VertexType/dynVertex.h"
#include "../MeshType/dynMesh.h"
#include "../model/model.h"

#ifndef MESH_H
#define MESH_H

Mesh buildMesh(dynVertex someVertex, dynIndex indicies, dynTexture someTexture);
void setupMesh(Mesh* mesh);
void drawModel(Mesh *mesh, Model* model);

#endif
