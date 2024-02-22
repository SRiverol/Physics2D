#include "dynMesh.h"
#include "stdlib.h"
#include <stdio.h>

dynMesh initdynMesh() {
  dynMesh hi;
  hi.total = 0;
  hi.size = 0;
  hi.items = NULL;
  return hi;
}
int getdynMeshSize(dynMesh *dynMesh) { return dynMesh->size; }

int getdynMeshTotal(dynMesh *dynMesh) { return dynMesh->total; }

void adddynMesh(dynMesh *dynMesh, Mesh newData) {
  dynMesh->total++;
  dynMesh->items =
      realloc(dynMesh->items, sizeof(Mesh) * dynMesh->total);

  if (dynMesh->items == NULL) {
    printf("memory is bad\n");
    exit(1);
  }

  dynMesh->size = sizeof(Mesh) * dynMesh->total;
  dynMesh->items[(dynMesh->total - 1)] = newData;
}

void insertdynMesh(dynMesh *dynMesh, unsigned int index,
                      Mesh insertData) {
  dynMesh->total++;
  dynMesh->items =
      realloc(dynMesh->items, sizeof(Mesh) * dynMesh->total);

  if (dynMesh->items == NULL) {
    printf("memory is bad but inserting\n");
    exit(1);
  }

  for (int i = dynMesh->total - 1; i > index; i--) {
    dynMesh->items[i] = dynMesh->items[i - 1];
  }
  dynMesh->items[index] = insertData;
  dynMesh->size = sizeof(Mesh) * dynMesh->total;
}

