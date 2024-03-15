#include "model.h"
#include "../../stb_image.h"
#include "../MeshType/dynMesh.h"
#include "../mesh/mesh.h"
#include <assimp/cimport.h> // Plain-C interface
#include <assimp/defs.h>
#include <assimp/material.h>
#include <assimp/mesh.h>
#include <assimp/postprocess.h> // Post processing flags
#include <assimp/scene.h>       // Output data structure
#include <assimp/types.h>
#include <cglm/types.h>
#include <cglm/vec2.h>
#include <cglm/vec3.h>
#include <cglm/vec4.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

dynTexture loadMaterialTextures(const struct aiMaterial *mat,
                                enum aiTextureType type, char *typeName,
                                Model *model);

void setupModel(Model *model, char *filePath, char *directory,
                Shader someModelShader) {
  model->meshes = initdynMesh();

  model->filePath = (char *)malloc(sizeof(char) * strlen(filePath));
  strcpy(model->filePath, filePath);

  model->modelShader = someModelShader;
  loadModel(model);
}

void draw(Model *model, Shader *shader) {


  for (unsigned int i = 0; i < model->meshes.total; i++) {
    drawModel(&model->meshes.items[i], shader);
  }
}

void loadModel(Model *model) {
  const struct aiScene *scene = aiImportFile(
      model->filePath, aiProcess_Triangulate | aiProcess_GenSmoothNormals |
                           aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !scene->mRootNode) {
    printf("[ASSIMP ERROR]: %s \n", aiGetErrorString());
    exit(1);
  } else {
  }

  sceneProcesssing(model, scene);
  aiReleaseImport(scene);
}

void sceneProcesssing(Model *model, const struct aiScene *scene) {

  for (unsigned int i = 0; i < scene->mRootNode->mNumMeshes; i++) {

    struct aiMesh *meshPtr = scene->mMeshes[scene->mRootNode->mMeshes[i]];
    adddynMesh(&model->meshes, processMesh(model, meshPtr, scene));
  }
  for (unsigned int i = 0; i < scene->mRootNode->mNumChildren; i++) {
    processNode(model, scene->mRootNode->mChildren[i], scene);
  }
}

Mesh processMesh(Model *model, const struct aiMesh *meshPtr,
                 const struct aiScene *scene) {

  Mesh returnMesh;
  dynTexture textures = initdynTexture();
  dynVertex vertices = initdynVertex();
  dynIndex indices = initDynIndex();

  // VERTEX LOOP
  for (unsigned int i = 0; i < meshPtr->mNumVertices; i++) {
    Vertex vertex;
    // GET POSITION COORDS PER MESH VEC3
    vec3 pos = {meshPtr->mVertices[i].x, meshPtr->mVertices[i].y,
                meshPtr->mVertices[i].z};
    glm_vec3_copy(pos, vertex.normal);

    // GET VERTEX NORMALS PER MESH VEC3
    vec3 normans = {meshPtr->mNormals[i].x, meshPtr->mNormals[i].y,
                    meshPtr->mNormals[i].z};
    glm_vec3_copy(normans, vertex.normal);

    // GET TEXTURE COORDS PER MESH VEC2
    if (meshPtr->mTextureCoords[0]) {
      vec2 texCoords = {meshPtr->mTextureCoords[0][i].x,
                        meshPtr->mTextureCoords[0][i].y};
      glm_vec2_copy(texCoords, vertex.texCoords);
    } else {
      glm_vec2_copy((vec2){0.0f, 0.0f}, vertex.texCoords);
    }
    adddynVertex(&vertices, vertex);
  }

  // INDEX LOOP
  for (unsigned int i = 0; i < meshPtr->mNumFaces; i++) {
    const struct aiFace face = meshPtr->mFaces[i];
    for (unsigned int j = 0; j < face.mNumIndices; j++)
      addDynIndex(&indices, face.mIndices[j]);
  }

  // TEXTURE LOOPS
  if (meshPtr->mMaterialIndex >= 0) {
    const struct aiMaterial *material =
        scene->mMaterials[meshPtr->mMaterialIndex];

    // ADD ALL DIFFUSE MAPS TO END OF TEXTURES IN ORDER
    dynTexture diffuseMaps = loadMaterialTextures(
        material, aiTextureType_DIFFUSE, "texture_diffuse", model);
    for (int i = 0; i < diffuseMaps.total; i++) {
      adddynTexture(&textures, diffuseMaps.items[i]);
    }

    // ADD ALL SPECULAR MAPS TO END OF TEXTURES IN ORDER
    dynTexture specularMaps = loadMaterialTextures(
        material, aiTextureType_SPECULAR, "texture_specular", model);
    for (int i = 0; i < diffuseMaps.total; i++) {
      adddynTexture(&textures, specularMaps.items[i]);
    }
  }
  returnMesh.textures = textures;
  returnMesh.indicies = indices;
  returnMesh.verticies = vertices;
  setupMesh(&returnMesh);
  // printf("returning a mesh \n");
  return returnMesh;
}

void processNode(Model *model, const struct aiNode *node,
                 const struct aiScene *scene) {
  // process all the node's meshes (if any)
  for (unsigned int i = 0; i < node->mNumMeshes; i++) {
    struct aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    adddynMesh(&model->meshes, processMesh(model, mesh, scene));
  }
  // then do the same for each of its children
  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    processNode(model, node->mChildren[i], scene);
  }
  // printf("Buildin a node \n");
}

dynTexture loadMaterialTextures(const struct aiMaterial *mat,
                                enum aiTextureType type, char *typeName,
                                Model *model) {

  extern dynTexture loaded_textures;
  dynTexture textures = initdynTexture();
  for (int i = 0; i < aiGetMaterialTextureCount(mat, type); i++) {
    struct aiString str;
    bool skip = false;

    for (unsigned int j = 0; j < loaded_textures.total; j++) {

      if (strcmp(loaded_textures.items[j].path, str.data) == 0) {
        adddynTexture(&textures, loaded_textures.items[j]);
        skip = true;
        break;
      }
    }

    if (!skip) {
      Texture texture;
      texture.type = malloc(sizeof(char) * strlen(typeName) + 1);
      texture.path = malloc(sizeof(char) * strlen(str.data) + 1);
      texture.id = textureFromFile(model->filePath, model->directory);
      strcpy(texture.type, typeName);
      strcpy(texture.path, str.data);
      adddynTexture(&textures, texture);
      adddynTexture(&loaded_textures, texture);
    }
  }
  return textures;
}

unsigned int textureFromFile(const char *path, const char *directory) {
  // // Allocate memory for finalPath
  // if (!directory || !path) {
  //   printf("Error: One of the input paths is null.\n");
  //   return 0; // or any error value
  // }
  // size_t finalPathSize =
  //     strlen(directory) + strlen(path) + 2; // 2 for '/' and null
  //     terminator
  // char *finalPath = (char *)malloc(finalPathSize);
  // if (!finalPath) {
  //   printf("Failed to allocate memory for finalPath");
  //   return 0; // Return 0 or any other error value to indicate failure
  // }
  //
  // // Concatenate directory, "/", and path to form finalPath
  // snprintf(finalPath, finalPathSize, "%s/%s", directory, path);
  //
  unsigned int textureID;
  glGenTextures(1, &textureID);

  int width, height, nrComponents;
  unsigned char *data = stbi_load(
      "/home/srl/Programming/Physics2D/assets/models/backpack/specular.jpg",
      &width, &height, &nrComponents, 0);
  if (data) {
    GLenum format;
    if (nrComponents == 1)
      format = GL_RED;
    else if (nrComponents == 3)
      format = GL_RGB;
    else if (nrComponents == 4)
      format = GL_RGBA;

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
  } else {
    printf("Failed to load texture at path: %s \n", path);
    stbi_image_free(data);
  }

  return textureID;
}
