#include "body.h"
#include "world.h"
#include "Integrator.h"

void Step(btBody* body, float timeStep)
{
	body->force = Vector2Add(body->force, Vector2Scale(Vector2Scale(btGravity, body->gravityScale), body->mass));
	body->acceleration = Vector2Scale(body->force, body->inverseMass);

	SemiImplicitEuler(body, timeStep);

	// Apply damping
	float damping = 1 / (1 + (body->damping * timeStep));
	body->velocity = Vector2Scale(body->velocity, damping);

	ClearForce(body);
}