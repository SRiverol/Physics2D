#include "camera.h"
#include <cglm/cam.h>
#include <cglm/cglm.h>
#include <cglm/mat4.h>
#include <cglm/vec3.h>

#include <math.h>
#include <stdio.h>
// Defines several possible options for camera movement-> Used as abstraction to
// stay away from window-system specific input methods

// Default camera values


// An abstract camera class that processes input and calculates the
// corresponding Euler Angles, Vectors and Matrices for use in OpenGL

// returns the view matrix calculated using Euler Angles and the LookAt Matrix
void getViewMatrix(Camera *camera) {
  vec3 crossView;

  glm_vec3_add(camera->Pos, camera->Front, crossView);
  glm_lookat(camera->Pos, crossView, camera->Up, camera->viewMatrix);

  
}

void editCamera(vec3 cameraPos, vec3 worldup, vec3 front, vec3 up, float yaw, float pitch,
                float speed, float sensitivity, float zoom, Camera *dest) {

  dest->Yaw = yaw;
  dest->Pitch = pitch;
  dest->movementSpeed = speed;
  dest->mouseSensitivity = sensitivity;
  dest->Zoom = zoom;


  glm_vec3_copy(up, dest->Up);
  glm_vec3_copy(front, dest->Front);
  glm_vec3_copy(worldup, dest->WorldUp);
  glm_vec3_copy(cameraPos, dest->Pos);

  updateCameraVectors(dest);
}
// processes input received from any keyboard-like input system-> Accepts input
// parameter in the form of camera defined ENUM (to abstract it from windowing
// systems)
void processKeyboard(Camera *camera, enum cameraMovement direction,
                     float deltaTime) {

  vec3 temp = {0.0f, 0.0f, 0.0f};


  float cameraSpeed = camera->movementSpeed * deltaTime;

  if (direction == FORWARD) {
    // printf("camera->Front: %f\n", *camera->Front);

    glm_vec3_crossn(camera->Right, (vec3) {0.0f, -1.0f, 0.0f}, temp);
    glm_vec3_muladds(temp, cameraSpeed, camera->Pos);

    
    // printf("FRONT X: %f, Y: %f, Z: %f \n", camera->Pos[0], camera->Pos[1],
           // camera->Pos[2]);
  }
  if (direction == BACKWARD) {
    glm_vec3_crossn(camera->Right, (vec3) {0.0f, -1.0f, 0.0f}, temp);
    glm_vec3_scale(temp, cameraSpeed, temp);
    glm_vec3_sub(camera->Pos, temp, camera->Pos);

    // printf("BACKWARD X: %f, Y: %f, Z: %f \n", camera->Pos[0], camera->Pos[1],
           // camera->Pos[2]);
  }
  if (direction == LEFT) {
    glm_vec3_crossn(camera->Front, camera->Up, temp);
    glm_vec3_scale(temp, cameraSpeed, camera->Left);
    glm_vec3_sub(camera->Pos, camera->Left, camera->Pos);

    // printf("LEFT X: %f, Y: %f, Z: %f \n", camera->Pos[0], camera->Pos[1],
           // camera->Pos[2]);
  }
  if (direction == RIGHT) {
    glm_vec3_crossn(camera->Front, camera->Up, temp);
    glm_vec3_scale(temp, cameraSpeed, temp);
    glm_vec3_add(camera->Pos, temp, camera->Pos);

    // printf("RIGHT X: %f, Y: %f, Z: %f \n", camera->Pos[0], camera->Pos[1],
           // camera->Pos[2]);
  }
  if (direction == UP) {

    glm_vec3_crossn( (vec3) {0.0f, 0.0f, -1.0f}, (vec3) {-1.0f, 0.0f, 0.0f}, temp);
    glm_vec3_scale(temp, cameraSpeed, temp);
    glm_vec3_add(camera->Pos, temp, camera->Pos);

    // printf("RIGHT X: %f, Y: %f, Z: %f \n", camera->Pos[0], camera->Pos[1],
           // camera->Pos[2]);
  }
  if (direction == DOWN) {


    glm_vec3_crossn( (vec3) {0.0f, 0.0f, 1.0f}, (vec3) {1.0f, 0.0f, 0.0f}, temp);
    glm_vec3_scale(temp, cameraSpeed *-1, temp);
    glm_vec3_add(camera->Pos, temp, camera->Pos);

    // printf("RIGHT X: %f, Y: %f, Z: %f \n", camera->Pos[0], camera->Pos[1],
    //        camera->Pos[2]);
  }

        if(camera->Pos[1] <= 0) {
                camera->Pos[1] = 0;
        }

}
// processes input received from a mouse scroll-wheel event-> Only requires
// input on the vertical wheel-axis
void processMouseScroll(Camera *camera, float yoffset) {
  camera->Zoom -= (float)yoffset;
  if (camera->Zoom < 1.0f)
    camera->Zoom = 1.0f;
  if (camera->Zoom > 45.0f)
    camera->Zoom = 45.0f;
}

// processes input received from a mouse input system-> Expects the offset value
// in both the x and y direction->
void processMouseMovement(Camera *camera, float xoffset, float yoffset,
                          GLboolean constrainPitch) {
  xoffset *= camera->mouseSensitivity;
  yoffset *= camera->mouseSensitivity;

  camera->Yaw += xoffset;
  camera->Pitch += yoffset;

  // make sure that when pitch is out of bounds, screen doesn't get flipped
  if (constrainPitch) {
    if (camera->Pitch > 89.0f)
      camera->Pitch = 89.0f;
    if (camera->Pitch < -89.0f)
      camera->Pitch = -89.0f;
  }

  // update Front, Right and Up Vectors using the updated Euler angles
  updateCameraVectors(camera);
}
// calculates the front vector from the Camera's (updated) Euler Angles
void updateCameraVectors(Camera *camera) {
        
  vec3 direction = {cos(glm_rad(camera->Yaw)) * cos(glm_rad(camera->Pitch)),
                    sin(glm_rad(camera->Pitch)),
                    sin(glm_rad(camera->Yaw)) * cos(glm_rad(camera->Pitch))};
  glm_normalize_to(direction, camera->Front);

        //RIGHT AND UP VECTOR
        vec3 crossRight;
        vec3 crossUp;
        glm_cross(camera->Front, camera->WorldUp, crossRight);
        glm_normalize_to(crossRight, camera->Right);

        glm_cross(camera->Right, camera->Front, crossUp);
        glm_normalize_to(crossUp, camera->Up);
}


