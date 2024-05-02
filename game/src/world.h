#pragma once
#include "raylib.h"

typedef struct btBody btBody; // Forward declaration

extern btBody* btBodies;
extern int btBodyCount;
extern Vector2 btGravity;

btBody* CreateBody();
void DestroyBody(btBody* body);
void DestroyAllBodies();