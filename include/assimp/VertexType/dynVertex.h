#ifndef dynVertex_H
#define dynVertex_H
#include <cglm/vec3.h>

typedef struct {
        vec3 position;
        vec3 normal;
        vec3 texCoords;
} Vertex;

typedef struct dynVertex{
        int size;
        int total;
        Vertex* items;
} dynVertex;

int getdynVertexSize(dynVertex*);
int getdynVertexTotal(dynVertex*);
void expanddynVertex(dynVertex*);
void insertdynVertex(dynVertex *dynVertex, unsigned int index,
                    Vertex insertData);

#endif
