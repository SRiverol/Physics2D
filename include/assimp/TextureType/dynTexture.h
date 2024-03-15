#ifndef dynTexture_H
#define dynTexture_H

typedef struct {
        unsigned int id;
        char* type;
        char* path;
} Texture;

typedef struct dynTexture{
        int size;
        int total;
        Texture* items;
} dynTexture;

dynTexture initdynTexture();
int getdynTextureSize(dynTexture*);
int getdynTextureTotal(dynTexture*);
void expanddynTexture(dynTexture*);

void adddynTexture(dynTexture *dynTexture, Texture newData);
void insertdynTexture(dynTexture *dynTexture, unsigned int index,
                      Texture insertData); 
#endif
