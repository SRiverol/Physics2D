#include "physicsObject.h"
#include <cglm/vec3.h>
#include <stdbool.h>

static void freeFall(PhysicsObject *object, float time);
void editphysicsParameters(PhysicsObject *object, vec3 position, float mass,
                           float verticalVelocity, float horizontalVelocity,
                           float verticalAcceleration,
                           float horizontalAcceleration) {
  object->mass = mass;
  object->verticalVelocity = verticalVelocity;
  object->horizontalVelocity = horizontalVelocity;
  object->verticalAcceleration = verticalVelocity;
  object->horizontalAcceleration = horizontalVelocity;
  glm_vec3_copy(position, object->position);
}

void initialAtRestPhysics(PhysicsObject *object, vec3 position, float mass) {
  editphysicsParameters(object, position, mass, 0, 0, -10, 0);
}

// PASS IN ARRAY OF POINTERS TO PHYSICS OBJECTS
// WILL PREFORM PHYSICS CALCULATIONS ON EACH
// BUT FOR NOW WE WILL ONLY USE ONE
void physicsRuntime(PhysicsObject *object, float time) {

  freeFall(object, time);
}

static void freeFall(PhysicsObject *object, float time) {


  object->verticalAcceleration = 0.45f * object->mass;
  // terminal velocity maybe we will add the formula here someday

  // if () {
  //   editphysicsParameters(object, object->position, object->mass, 0, 0, 0,
  //   0);
  // }

  if (object->verticalVelocity >= 5) {
    object->verticalAcceleration = 0;
  }

  object->verticalVelocity -= object->verticalAcceleration * time;

  // very fancy collission guys
  if (object->position[1] <= 0) {
    object->verticalVelocity = 0;

    object->position[1] = 0;
  }


  object->position[1] += object->verticalVelocity * time;
}
