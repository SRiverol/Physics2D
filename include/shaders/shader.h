#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include <cglm/vec3.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct {
    unsigned int shaderID;
} Shader;

static void checkCompileErrors(unsigned int shader, char *type);

Shader buildShaders(char *vertexPath, char *fragmentPath);


// Shader Commands
void useShader(Shader shader);
void shaderSetBool(Shader shader, const char *name, bool value);
void shaderSetInt(Shader shader, const char *name, int value);
void shaderSetFloat(Shader shader, const char *name, float value);
void shaderSetVec3(Shader shader, const char *name, const vec3 value);
void shaderSetMat4(Shader shader, const char *name, const mat4 value);
