#include "dynIndex.h"
#include "stdlib.h"
#include <stdio.h>

dynIndex initDynIndex() {
  dynIndex hi;
  hi.total = 0;
  hi.size = 0;
  hi.items = NULL;
  return hi;
}
int getDynIndexSize(dynIndex *dynIndex) { return dynIndex->size; }

int getDynIndexTotal(dynIndex *dynIndex) { return dynIndex->total; }

void addDynIndex(dynIndex *dynIndex, unsigned int newData) {
  dynIndex->total++;
  dynIndex->items =
      realloc(dynIndex->items, sizeof(unsigned int) * dynIndex->total);

  if (dynIndex->items == NULL) {
    printf("memory is bad\n");
    exit(1);
  }

  dynIndex->size = sizeof(unsigned int) * dynIndex->total;
  dynIndex->items[(dynIndex->total - 1)] = newData;
}

void insertDynIndex(dynIndex *dynIndex, unsigned int index,
                    unsigned int insertData) {
  dynIndex->total++;
  dynIndex->items =
      realloc(dynIndex->items, sizeof(unsigned int) * dynIndex->total);

  if (dynIndex->items == NULL) {
    printf("memory is bad but inserting\n");
    exit(1);
  }

  for (int i = dynIndex->total - 1; i > index; i--) {
    dynIndex->items[i] = dynIndex->items[i - 1];
  }
  dynIndex->items[index] = insertData;
  dynIndex->size = sizeof(unsigned int) * dynIndex->total;
}
