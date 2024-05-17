#pragma once
#include "raylib.h"
#include "raymath.h"

typedef enum
{
	BT_STATIC,
	BT_KINEMATIC,
	BT_DYNAMIC
} btBodyType;

typedef enum
{
	FM_FORCE,
	FM_IMPULSE,
	FM_VELOCITY
} btForceMode;



typedef struct btBody
{
	btBodyType type;

	// acceleration -> velocity -> position
	Vector2 position;
	Vector2 velocity;
	Vector2 acceleration;
	Vector2 force;

	float mass;
	float inverseMass; // 1 / mass (static = 0)
	float gravityScale;
	float damping;

	float restitution;

	Color color;

	struct btBody* next;
	struct btBody* prev;

} btBody;

inline void ApplyForce(btBody* body, Vector2 force, btForceMode forceMode)
{
	if (body->type != BT_DYNAMIC) return;

	switch (forceMode)
	{
	case FM_FORCE:
		body->force = Vector2Add(body->force, force);
		break;
	case FM_IMPULSE:
		// Applies a sudden change in velocity
		body->velocity = Vector2Add(body->velocity, Vector2Scale(force, body->inverseMass));
		break;
	case FM_VELOCITY:
		body->velocity = force;
		break;
	}
}

inline void ClearForce(btBody* body)
{
	body->force = Vector2Zero();
}

void Step(btBody* body, float timeStep);