#define STB_IMAGE_IMPLEMENTATION

#include "../include/assimp/model/model.h"
#include "../include/cameras/camera.h"
#include "../include/physics/physicsObject.h"
#include "../include/stb_image.h"
#include <cglm/affine.h>
#include <cglm/cam.h>
#include <cglm/mat4.h>
#include <cglm/types.h>
#include <cglm/util.h>
#include <cglm/vec3.h>
#include <stdio.h>
#include <stdlib.h>

#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
unsigned int loadTexture(char *path);

// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

dynTexture loaded_textures;
PhysicsObject mainCube;
// camera
Camera mainCamera;
// Default Main Cam Values
vec3 pos = {0.0f, 0.0f, 0.0f};
vec3 worldup = {0.0f, 1.0f, 0.0f};
vec3 front = {0.0f, 0.0f, -1.0f};
vec3 up = {0.0f, 1.0f, 0.0f};
// default camera values
float yaw = -90.0f;
float pitch = 0.0f;
float movementSpeed = 10.0f;
float mouseSensitivity = 0.1f;
float Zoom = 45.0f;

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting
vec3 lightPos = {1.2f, 1.0f, 2.0f};
vec3 lightColor = {1.0f, 1.0f, 1.0f};
vec3 toyColor = {1.0f, 0.5f, 0.31f};

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // glfw window creation
  // --------------------
  GLFWwindow *window =
      glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Cubes are cool", NULL, NULL);
  if (window == NULL) {
    printf("WINDOW FAIL \n");
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  // glad: load all OpenGL function pointers
  // ---------------------------------------
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    printf("GLAD FAIL \n");

    return -1;
  }
  // Global settings
  glEnable(GL_DEPTH_TEST);

  // Camera Setup
  editCamera(pos, worldup, front, up, yaw, pitch, movementSpeed,
             mouseSensitivity, Zoom, &mainCamera);
  Model fancyModel;

  // Shader definitions
  Shader lightShader =
      buildShaders("/home/srl/Programming/Physics2D/src/colors/colors.vs",
                   "/home/srl/Programming/Physics2D/src/colors/colors.fs");

  Shader basic =
      buildShaders("/home/srl/Programming/Physics2D/src/basicShader/basic.vs",
                   "/home/srl/Programming/Physics2D/src/basicShader/basic.fs");

  Shader lightCubeSourceShader = buildShaders(
      "/home/srl/Programming/Physics2D/src/lightCubeSource/lightCube.vs",
      "/home/srl/Programming/Physics2D/src/lightCubeSource/lightCube.fs");

  setupModel(
      &fancyModel,
      "/home/srl/Programming/Physics2D/assets/models/testCube/untitled.obj",
      "Yo", lightShader);
  printf("Loading Model Complete \n");

  float vertices[] = {
      // positions          // normals           // texture coords
      -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  0.5f,  -0.5f,
      -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f,  0.0f,  0.5f,  0.5f,  -0.5f, 0.0f,
      0.0f,  -1.0f, 1.0f,  1.0f,  0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f,
      1.0f,  1.0f,  -0.5f, 0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f,  1.0f,
      -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,

      -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.5f,  -0.5f,
      0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  0.0f,
      0.0f,  1.0f,  1.0f,  1.0f,  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
      1.0f,  1.0f,  -0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
      -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

      -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  1.0f,  0.0f,  -0.5f, 0.5f,
      -0.5f, -1.0f, 0.0f,  0.0f,  1.0f,  1.0f,  -0.5f, -0.5f, -0.5f, -1.0f,
      0.0f,  0.0f,  0.0f,  1.0f,  -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,
      0.0f,  1.0f,  -0.5f, -0.5f, 0.5f,  -1.0f, 0.0f,  0.0f,  0.0f,  0.0f,
      -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  1.0f,  0.0f,

      0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.5f,  0.5f,
      -0.5f, 1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  0.5f,  -0.5f, -0.5f, 1.0f,
      0.0f,  0.0f,  0.0f,  1.0f,  0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
      0.0f,  1.0f,  0.5f,  -0.5f, 0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
      0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

      -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f,  1.0f,  0.5f,  -0.5f,
      -0.5f, 0.0f,  -1.0f, 0.0f,  1.0f,  1.0f,  0.5f,  -0.5f, 0.5f,  0.0f,
      -1.0f, 0.0f,  1.0f,  0.0f,  0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,
      1.0f,  0.0f,  -0.5f, -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  0.0f,  0.0f,
      -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f,  1.0f,

      -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.5f,  0.5f,
      -0.5f, 0.0f,  1.0f,  0.0f,  1.0f,  1.0f,  0.5f,  0.5f,  0.5f,  0.0f,
      1.0f,  0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
      1.0f,  0.0f,  -0.5f, 0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
      -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f,  1.0f};

  vec3 cubePositions[] = {
      {2.0f, 5.0f, -15.0f}, {-1.5f, -2.2f, -2.5f}, {-3.8f, -2.0f, -12.3f},
      {2.4f, -0.4f, -3.5f}, {-1.7f, 3.0f, -7.5f},  {1.3f, -2.0f, -2.5f},
      {1.5f, 2.0f, -2.5f},  {1.5f, 0.2f, -1.5f},   {-1.3f, 1.0f, -1.5f}};

  vec3 pointLightPositions[] = {{0.7f, 0.2f, 2.0f},
                                {2.3f, -3.3f, -4.0f},
                                {-4.0f, 2.0f, -12.0f},
                                {0.0f, 0.0f, -3.0f}};

  unsigned int VBO, cubeVAO;

  glGenVertexArrays(1, &cubeVAO);

  glGenBuffers(1, &VBO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindVertexArray(cubeVAO);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  unsigned int lightCubeSourceVAO;

  glGenVertexArrays(1, &lightCubeSourceVAO);
  glBindVertexArray(lightCubeSourceVAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  unsigned int diffuseMap = loadTexture(
      "/home/srl/Programming/lunarpenguin/Resources/LunarPenguin.png");
  unsigned int specularMap =
      loadTexture("/home/srl/Pictures/LunarPenguin_Spec.png");
  unsigned int diffuseMapD = loadTexture("/home/srl/Pictures/dirt.png");

  // shader configuration
  // --------------------
  useShader(lightShader);
  shaderSetInt(lightShader, "material.diffuse", 0);
  shaderSetInt(lightShader, "material.specular", 1);

  // CUBE INFO
  initialAtRestPhysics(&mainCube, (vec3){0.0f, 24.0f, 0.0f}, 1);

  // render loop
  // -----------
  while (!glfwWindowShouldClose(window)) {
    // PER FRAME LOGIC
    //------------------
    float currentFrame = (float)glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // input
    // -----------------
    processInput(window);
    physicsRuntime(&mainCube, deltaTime);

    // render
    // -----------------
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    useShader(lightShader);

    glm_vec3_copy(
        (vec3){
            (float)sin(glfwGetTime() / 1) * 6,
            (float)cos(glfwGetTime() / 1) * 6,
            (float)cos(glfwGetTime() / 3) * 6,
        },
        lightPos);
    // COLORFUL LIGHTS
    glm_vec3_copy((vec3){(float)(sin(glfwGetTime() * 1)),
                         (float)(cos(glfwGetTime() / 2)),
                         (float)(cos(glfwGetTime() / 4))},
                  lightColor);
    vec3 ambientLight;
    glm_vec3_mul(lightColor, (vec3){0.2f, 0.2f, 0.2f}, ambientLight);
    // MATERIAL PROPERTIES
    shaderSetFloat(lightShader, "material.shininess", 64.0f);
    shaderSetVec3(lightShader, "objectColor", toyColor);

    // LIGHT PROPERTIES

    shaderSetVec3(lightShader, "lightColor", lightColor);
    shaderSetVec3(lightShader, "globallight.ambient", (vec3){0.2f, 0.2f, 0.2f});
    shaderSetVec3(lightShader, "globallight.diffuse", (vec3){0.0f, 0.0f, 0.0f});
    shaderSetVec3(lightShader, "globallight.specular",
                  (vec3){0.0f, 0.0f, 0.0f});
    shaderSetVec3(lightShader, "pointLights[0].ambient", ambientLight);
    shaderSetVec3(lightShader, "pointLights[0].diffuse", lightColor);
    shaderSetVec3(lightShader, "pointLights[0].specular", lightColor);

    // POINT LIGHT
    shaderSetVec3(lightShader, "pointLights[0].position", lightPos);

    shaderSetFloat(lightShader, "pointLights[0].constant", 1.0f);
    shaderSetFloat(lightShader, "pointlights[0].linear", 0.09f);
    shaderSetFloat(lightShader, "pointLights[0].quadratic", 0.032f);

    // GLOBAL LIGHT
    shaderSetVec3(lightShader, "globallight.direction",
                  (vec3){0.0f, -1.0f, -0.5f});

    // Camera land
    // -----------------
    mat4 projection;
    mat4 view;
    glm_perspective(glm_rad(mainCamera.Zoom),
                    (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f,
                    projection);
    getViewMatrix(&mainCamera);
    glm_mat4_copy(mainCamera.viewMatrix, view);

    shaderSetMat4(lightShader, "view", view);
    shaderSetMat4(lightShader, "projection", projection);
    // World Transformation
    mat4 model;
    glm_mat4_identity(model);

    glm_scale(model, (vec3){10.1f, 0.1f, 10.1f});
    glm_translate(model, (vec3){0.0f, -7.5f, 0.0f});

    shaderSetMat4(lightShader, "fragModel", model);
    // glm_rotate(model, glfwGetTime(), (vec3){5.0f, 5.0f, 0.0f});
    shaderSetMat4(lightShader, "model", model);
    shaderSetVec3(lightShader, "viewPos", mainCamera.Pos);

    // the exalted colored by light cube of 0,0,0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularMap);
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glm_mat4_identity(model);

    glm_translate(model, mainCube.position);
    shaderSetMat4(lightShader, "model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    // lame cube light source or whatever
    // Lame light stuff
    vec3 norm;
    vec3 lightDirection;

    useShader(lightCubeSourceShader);
    shaderSetMat4(lightCubeSourceShader, "projection", projection);
    shaderSetMat4(lightCubeSourceShader, "view", view);
    shaderSetVec3(lightCubeSourceShader, "color", lightColor);
    glm_mat4_identity(model);
    glm_translate(model, lightPos);
    glm_scale(model, (vec3){0.1f, 0.1f, 0.1f});
    shaderSetMat4(lightCubeSourceShader, "model", model);

    draw(&fancyModel, &lightShader);
    shaderSetInt(lightShader, "material.diffuse", 2);
    glBindVertexArray(lightCubeSourceVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Events
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // Terminate
  glDeleteVertexArrays(1, &cubeVAO);
  glDeleteVertexArrays(1, &lightCubeSourceVAO);
  glDeleteBuffers(1, &VBO);
  glfwTerminate();
  return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this
// frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    processKeyboard(&mainCamera, FORWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    processKeyboard(&mainCamera, BACKWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    processKeyboard(&mainCamera, LEFT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    processKeyboard(&mainCamera, RIGHT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    processKeyboard(&mainCamera, UP, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    processKeyboard(&mainCamera, DOWN, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    initialAtRestPhysics(&mainCube, (vec3){0.0f, 24.0f, 0.0f}, 300);
  if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

// glfw: whenever the window size changed (by OS or user resize) this callback
// function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  // make sure the viewport matches the new window dimensions; note that width
  // and height will be significantly larger than specified on retina
  // displays.
  glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn) {
  float xpos = (float)(xposIn);
  float ypos = (float)(yposIn);

  if (firstMouse) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset =
      lastY - ypos; // reversed since y-coordinates go from bottom to top

  lastX = xpos;
  lastY = ypos;

  processMouseMovement(&mainCamera, xoffset, yoffset, true);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
  processMouseScroll(&mainCamera, (float)yoffset);
}

unsigned int loadTexture(char *path) {
  unsigned int textureID;
  glGenTextures(1, &textureID);

  int width, height, nrComponents;
  unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 4);
  if (data != NULL) {
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
    printf("Texture failed to load at path: \n");
    stbi_image_free(data);
  }

  return textureID;
}
