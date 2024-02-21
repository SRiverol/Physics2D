#ifndef dynIndex_H
#define dynIndex_H

typedef struct dynIndex{
        int size;
        int total;
        unsigned int* items;
} dynIndex;

int getDynIndexSize(dynIndex*);
int getDynIndexTotal(dynIndex*);
void expandDynIndex(dynIndex*);

#endif
