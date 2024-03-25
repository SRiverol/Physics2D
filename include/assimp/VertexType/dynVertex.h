#ifndef dynVertex_H
#define dynVertex_H
#include <cglm/vec2.h>
#include <cglm/vec3.h>

typedef struct Vertex{
  vec3 position;
  vec3 normal;
  vec2 texCoords;
} Vertex;

typedef struct dynVertex {
  int size;
  int total;
  Vertex *items;
} dynVertex;

dynVertex initdynVertex();
int getdynVertexSize(dynVertex *);
int getdynVertexTotal(dynVertex *);
void expanddynVertex(dynVertex *);

void adddynVertex(dynVertex *dynVertex, Vertex newData);

void insertdynVertex(dynVertex *dynVertex, unsigned int index,
                     Vertex insertData);

#endif
