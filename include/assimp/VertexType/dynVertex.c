#include "dynVertex.h"
#include <stdio.h>

dynVertex initdynVertex() {
  dynVertex hi;
  hi.total = 0;
  hi.size = 0;
  hi.items = NULL;
  return hi;
}
int getdynVertexSize(dynVertex *dynVertex) { return dynVertex->size; }

int getdynVertexTotal(dynVertex *dynVertex) { return dynVertex->total; }

void adddynVertex(dynVertex *dynVertex, Vertex newData) {
  dynVertex->total++;
  dynVertex->items =
      realloc(dynVertex->items, sizeof(Vertex) * dynVertex->total);

  if (dynVertex->items == NULL) {
    printf("memory is bad\n");
    exit(1);
  }

  dynVertex->size = sizeof(Vertex) * dynVertex->total;
  dynVertex->items[(dynVertex->total - 1)] = newData;
}

void insertdynVertex(dynVertex *dynVertex, unsigned int index,
                    Vertex insertData) {
  dynVertex->total++;
  dynVertex->items =
      realloc(dynVertex->items, sizeof(Vertex) * dynVertex->total);

  if (dynVertex->items == NULL) {
    printf("memory is bad but inserting\n");
    exit(1);
  }

  for (int i = dynVertex->total - 1; i > index; i--) {
    dynVertex->items[i] = dynVertex->items[i - 1];
  }
  dynVertex->items[index] = insertData;
  dynVertex->size = sizeof(Vertex) * dynVertex->total;
}
