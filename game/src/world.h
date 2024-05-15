#pragma once
#include "raylib.h"
#include "body.h"

typedef struct btBody btBody; // Forward declaration

extern btBody* btBodies;
extern int btBodyCount;
extern Vector2 btGravity;

btBody* CreateBody(Vector2 position, float mass, btBodyType bodyType);
void AddBody(btBody* body);
void DestroyBody(btBody* body);
void DestroyAllBodies();