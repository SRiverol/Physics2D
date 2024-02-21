#include <cglm/vec3.h>
#include "../IndexType/dynIndex.h"
#include "../TextureType/dynTexture.h"
#include "../VertexType/dynVertex.h"

#ifndef MESH_H
#define MESH_H

typedef struct {
        dynVertex verticies;
        dynTexture textures;
        dynIndex indicies;
} Mesh;

Mesh buildMesh(dynVertex someVertex, dynIndex indicies, dynTexture someTexture);
void setupMesh(Mesh* mesh, unsigned int VAO);

#endif
