#ifndef CAMERA_H
#define CAMERA_H
#include <cglm/cglm.h>
#include <cglm/mat4.h>
#include <cglm/vec3.h>
#include <glad/glad.h>
#include <stdio.h>

enum cameraMovement { FORWARD, BACKWARD, LEFT, RIGHT, UP, DOWN };

typedef struct {
  // camera Attributes
  vec3 Pos;
  vec3 Front;
  vec3 Up;
  vec3 Right;
  vec3 Left;
  vec3 WorldUp;
  // euler Angles
  float Yaw;
  float Pitch;
  // camera options
  float movementSpeed;
  float mouseSensitivity;
  float Zoom;
  // View Matrix
  mat4 viewMatrix;
} Camera;

void getViewMatrix(Camera *camera);
void editCamera(vec3 cameraPos, vec3 worldup,vec3 Front, vec3 Up, float yaw, float pitch,
                float speed, float sensitivity, float zoom, Camera *dest);
void cameraPosition(float *hi);
void processKeyboard(Camera *camera, enum cameraMovement direction,
                     float deltaTime);
void processMouseMovement(Camera *camera, float xoffset, float yoffset,
                          GLboolean constrainPitch);
void processMouseScroll(Camera *camera, float yoffset);
void updateCameraVectors(Camera *camera);

#endif
