#include "dynTexture.h"
#include "stdlib.h"
#include <stdio.h>

dynTexture initdynTexture() {
  dynTexture hi;
  hi.total = 0;
  hi.size = 0;
  hi.items = NULL;
  return hi;
}
int getdynTextureSize(dynTexture *dynTexture) { return dynTexture->size; }

int getdynTextureTotal(dynTexture *dynTexture) { return dynTexture->total; }

void adddynTexture(dynTexture *dynTexture, Texture newData) {
  dynTexture->total++;
  dynTexture->items =
      realloc(dynTexture->items, sizeof(Texture) * dynTexture->total);

  if (dynTexture->items == NULL) {
    printf("memory is bad\n");
    exit(1);
  }

  dynTexture->size = sizeof(Texture) * dynTexture->total;
  dynTexture->items[(dynTexture->total - 1)] = newData;
}

void insertdynTexture(dynTexture *dynTexture, unsigned int index,
                      Texture insertData) {
  dynTexture->total++;
  dynTexture->items =
      realloc(dynTexture->items, sizeof(Texture) * dynTexture->total);

  if (dynTexture->items == NULL) {
    printf("memory is bad but inserting\n");
    exit(1);
  }

  for (int i = dynTexture->total - 1; i > index; i--) {
    dynTexture->items[i] = dynTexture->items[i - 1];
  }
  dynTexture->items[index] = insertData;
  dynTexture->size = sizeof(Texture) * dynTexture->total;
}
