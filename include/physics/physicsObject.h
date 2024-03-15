#ifndef PHYSICSOBJECT_H
#define PHYSICSOBJECT_H
#include <cglm/vec3.h>

typedef struct {
  vec4 position;
  unsigned int mass;
  int verticalAcceleration;
  int horizontalAcceleration;
  int verticalVelocity;
  int horizontalVelocity;
} PhysicsObject;

void editphysicsParameters(PhysicsObject *object, vec3 position, float mass,
                           float verticalVelocity, float horizontalVelocity,
                           float verticalAcceleration,
                           float horizontalAcceleration);

void initialAtRestPhysics(PhysicsObject *object, vec3 position, float mass);

void physicsRuntime(PhysicsObject *object, float time);
#endif
