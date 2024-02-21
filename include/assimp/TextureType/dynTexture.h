#ifndef dynTexture_H
#define dynTexture_H

typedef struct {
        unsigned int id;
        char* type;
} Texture;

typedef struct dynTexture{
        int size;
        int total;
        Texture* items;
} dynTexture;

int getdynTextureSize(dynTexture*);
int getdynTextureTotal(dynTexture*);
void expanddynTexture(dynTexture*);

#endif
