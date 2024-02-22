#include "mesh.h"
#include <glad/glad.h>

#include "../IndexType/dynIndex.h"
#include "../TextureType/dynTexture.h"
#include "../VertexType/dynVertex.h"
#include <stddef.h>

#include <GLFW/glfw3.h>
#include <cglm/vec3.h>
#include <stdio.h>
#include <string.h>

unsigned int VAO, VBO, EBO;

Mesh buildMesh(dynVertex someVertex, dynIndex indicies,
               dynTexture someTexture) {
  Mesh mesh;

  setupMesh(&mesh);

  return mesh;
};

void setupMesh(Mesh *mesh) {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, mesh->verticies.size,
               &(mesh->verticies.items[0]), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indicies.size,
               &(mesh->indicies.items[0]), GL_STATIC_DRAW);

  // vertex positions
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
  // vertex normals
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, normal));
  // vertex texture coords
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, texCoords));

  glBindVertexArray(0);
};

void drawModel(Mesh *mesh, Model* model) {
  unsigned int diffuseNr = 1;
  unsigned int specularNr = 1;
  for (unsigned int i = 0; i < mesh->textures.size; i++) {
    glActiveTexture(GL_TEXTURE0 +
                    i); // activate proper texture unit before binding
    // retrieve texture number (the N in diffuse_textureN)
    char number[20];
    char *name = mesh->textures.items[i].type;
    if (strcmp(name, "texture_diffuse")) {
      diffuseNr++;
      sprintf(number, "%i", diffuseNr);
    } else if (strcmp(name, "texture_specular")) {
      specularNr++;
      sprintf(number, "%i", diffuseNr);
    }
    shaderSetInt(model->modelShader, strcat("material.", name), i);
    glBindTexture(GL_TEXTURE_2D, mesh->textures.items[i].id);
  }
  glActiveTexture(GL_TEXTURE0);

  // draw mesh
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, mesh->indicies.size, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}
