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
  // // vertex texture coords
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, texCoords));

  glBindVertexArray(0);
};

void drawModel(Mesh *mesh, Shader *shader) {

  unsigned int diffuseNr = 1;
  unsigned int specularNr = 1;
  for (unsigned int i = 0; i < mesh->textures.total; i++) {
    glActiveTexture(GL_TEXTURE0 +
                    i); // activate proper texture unit before binding
    // retrieve texture number (the N in diffuse_textureN)
    char number[20];

    if (mesh->textures.items[i].type != NULL) {
      if (strncmp(mesh->textures.items[i].type, "texture_diffuse", 12) == 0) {
        diffuseNr++;
        sprintf(number, "%i", diffuseNr);
      } else if (strncmp(mesh->textures.items[i].type, "texture_specular",
                         12) == 0) {
        specularNr++;
        sprintf(number, "%i", specularNr);
      }

      glBindTexture(GL_TEXTURE_2D, mesh->textures.items[i].id);
    }
    glActiveTexture(GL_TEXTURE0);
  }
  // draw mesh
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, mesh->indicies.size, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}
