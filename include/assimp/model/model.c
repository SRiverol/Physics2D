#include "../mesh/mesh.h"
#include "model.h"
#include <string.h>
#include <assimp/cimport.h>        // Plain-C interface
#include <assimp/scene.h>          // Output data structure
#include <assimp/postprocess.h>    // Post processing flags

void setupModel(Model* model, char* filePath, dynMesh someMeshes, Shader someModelShader) {
        strcpy(model->filePath, filePath);
        model->meshes = someMeshes;
        model->modelShader = someModelShader;

        draw(model);
}

void draw(Model* model) {
        for(unsigned int i = 0; i < model->meshes.size; i++) {
                drawModel(&model->meshes.items[i], model);
                
        }
}

void loadModel(char* path) {

}
