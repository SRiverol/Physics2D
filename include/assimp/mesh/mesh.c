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

void setupMesh(Mesh *mesh) {

  unsigned int VAO, VBO, EBO;

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, mesh->verticies.total * sizeof(Vertex),
               &(mesh->verticies.items[0]), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               mesh->indicies.total * sizeof(unsigned int),
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
  mesh->VAO = VAO;
};

void drawModel(Mesh *mesh, Shader *shader) {

  printf("[VAO Num]: %i\n [Vertex]: %f, %f, %f\n", mesh->VAO,
         mesh->verticies.items[0].position[0],
         mesh->verticies.items[0].position[1],
         mesh->verticies.items[0].position[2]);
  unsigned int diffuseNr = 1;
  unsigned int specularNr = 1;

  for (unsigned int i = 0; i < mesh->textures.total; i++) {
    glActiveTexture(GL_TEXTURE0 +
                    i); // activate proper texture unit before binding
    // retrieve texture number (the N in diffuse_textureN)
    char number[300];
    if (strncmp(mesh->textures.items[i].type, "texture_diffuse", 12) == 0) {
      diffuseNr++;
      sprintf(number, "%i", diffuseNr);
    } else if (strncmp(mesh->textures.items[i].type, "texture_specular", 12) ==
               0) {
      specularNr++;
      sprintf(number, "%i", specularNr);
    }

    char parameter_name[100]; // Adjust size as needed
    snprintf(parameter_name, sizeof(parameter_name), "material.%s%s",
             mesh->textures.items[i].type, number);
    // printf("New Texture: %s has arrived! \n", parameter_name);

    glUniform1i(glGetUniformLocation(shader->shaderID, parameter_name), i);
    glBindTexture(GL_TEXTURE_2D, mesh->textures.items[i].id);
  }

  // draw mesh
  glBindVertexArray(mesh->VAO);
  glDrawElements(GL_TRIANGLES, (unsigned int)mesh->indicies.total,
                 GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);

  glActiveTexture(GL_TEXTURE0);
}
