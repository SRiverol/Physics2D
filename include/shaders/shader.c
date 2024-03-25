#include "shader.h"
#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include <cglm/vec3.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void checkCompileErrors(unsigned int shader, char *type) {
  int success;
  char infoLog[1024];
  if (!strcmp(type, "PROGRAM")) {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(shader, 1024, NULL, infoLog);
      printf("[%s SHADER COMPILE ERROR] %s \n", type, infoLog);
    }
  } else {
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(shader, 1024, NULL, infoLog);
      printf("[SHADER COMPILE ERROR] %s \n", infoLog);
    }
  }
}

Shader buildShaders(char *vertexPath, char *fragmentPath) {
  Shader finalShader;

  // Load shader data from file into program
  //--------------------------------------------------
  long vSize, fSize;
  FILE *vShaderFile, *fShaderFile;
  char *vShaderContent, *fShaderContent;

  fShaderFile = fopen(fragmentPath, "r");
  vShaderFile = fopen(vertexPath, "r");

  if (fShaderFile == NULL) {
    perror("FRAGMENT: Failed to open file.\n");
  }
  if (vShaderFile == NULL) {
    perror("VERTEX: Failed to open file.\n");
  }

  char *vShaderStream;
  char *fShaderStream;

  fseek(vShaderFile, 0L, SEEK_END);
  vSize = ftell(vShaderFile);
  fseek(fShaderFile, 0L, SEEK_END);
  fSize = ftell(fShaderFile);

  rewind(vShaderFile);
  rewind(fShaderFile);

  vShaderContent = malloc(vSize + 1);
  fShaderContent = malloc(fSize + 1);

  if (fShaderContent == NULL || vShaderContent == NULL) {
    printf("Memory allocation failed.\n");
    fclose(vShaderFile);
    fclose(fShaderFile);
  }

  if (vShaderContent == NULL || fShaderContent == NULL) {
    printf("Memory allocation failed");
    // Handle the failure, possibly by exiting the program
  }

  size_t vBytes = fread(vShaderContent, 1, vSize, vShaderFile);
  size_t fBytes = fread(fShaderContent, 1, fSize, fShaderFile);

  if (vBytes != vSize || fBytes != fSize) {
    printf("Error reading file.\n");
    fclose(vShaderFile);
    free(fShaderFile);
  }

  vShaderContent[vSize] = '\0';
  fShaderContent[fSize] = '\0';

  fclose(vShaderFile);
  fclose(fShaderFile);

  // Compile shader data into a shader program
  //--------------------------------------------------
  unsigned int vertex, fragment;
  // Vertex shader
  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vShaderContent, NULL);
  glCompileShader(vertex);
  checkCompileErrors(vertex, "VERTEX");
  // Fragment shader
  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fShaderContent, NULL);
  glCompileShader(fragment);
  checkCompileErrors(fragment, "FRAGMENT");
  // Shader program
  finalShader.shaderID = glCreateProgram();
  glAttachShader(finalShader.shaderID, vertex);
  glAttachShader(finalShader.shaderID, fragment);
  glLinkProgram(finalShader.shaderID);
  checkCompileErrors(finalShader.shaderID, "PROGRAM");
  // Clean up
  glDeleteShader(vertex);
  glDeleteShader(fragment);

  GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    // Log or handle the error
    printf("Error: %i", error);
  }
  return finalShader;
}

// Shader Commands

void useShader(Shader shader) { glUseProgram(shader.shaderID); }
// utility uniform functions
// ------------------------------------------------------------------------
void shaderSetBool(Shader shader, const char *name, bool value) {
  glUniform1i(glGetUniformLocation(shader.shaderID, name), value);
}
// ------------------------------------------------------------------------
void shaderSetInt(Shader shader, const char *name, int value) {
  glUniform1i(glGetUniformLocation(shader.shaderID, name), value);
}
// ------------------------------------------------------------------------
void shaderSetFloat(Shader shader, const char *name, float value) {
  glUniform1f(glGetUniformLocation(shader.shaderID, name), value);
}

void shaderSetVec3(Shader shader, const char *name, const vec3 value) {
  glUniform3fv(glGetUniformLocation(shader.shaderID, name), 1, value);
}

void shaderSetMat4(Shader shader, const char *name, const mat4 value) {
  glUniformMatrix4fv(glGetUniformLocation(shader.shaderID, name), 1, GL_FALSE,
                     (float *)value);
}
