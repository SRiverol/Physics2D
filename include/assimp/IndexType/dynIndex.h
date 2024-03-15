#ifndef dynIndex_H
#define dynIndex_H

typedef struct dynIndex{
        int size;
        int total;
        unsigned int* items;
} dynIndex;
dynIndex initDynIndex(); 
int getDynIndexSize(dynIndex*);
int getDynIndexTotal(dynIndex*);
void expandDynIndex(dynIndex*);
void addDynIndex(dynIndex *dynIndex, unsigned int newData);

#endif
