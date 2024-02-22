#ifndef dynMesh_H
#define dynMesh_H
#include "../VertexType/dynVertex.h"
#include "../IndexType/dynIndex.h"
#include "../TextureType/dynTexture.h"

typedef struct {
  dynVertex verticies;
  dynTexture textures;
  dynIndex indicies;
} Mesh;

typedef struct dynMesh {
  int size;
  int total;
  Mesh *items;
} dynMesh;

int getdynMeshSize(dynMesh *);
int getdynMeshTotal(dynMesh *);
void insertdynMesh(dynMesh *dynMesh, unsigned int index, Mesh insertData);

#endif
