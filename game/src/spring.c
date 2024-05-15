#include "spring.h"
#include "body.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

btSpring_t* btSprings = NULL;
int btSpringCount = 0;
Vector2 btGravity;

btSpring_t* CreateSpring(btBody* body1, btBody* body2, float restLength, float k)
{
	// Allocating memory for a new Body
	btSpring_t* spring = (btSpring_t*)malloc(sizeof(btSpring_t));
	// Assertion to check if the allocation actually worked
	assert(spring);

	// Clearing the memory of the body pointer
	memset(spring, 0, sizeof(btSpring_t));

	spring->body1 = body1;
	spring->body2 = body2;
	spring->restLength = restLength;
	spring->k = k;




	// Returns the new body
	return spring;
}

void AddSpring(btSpring_t* spring)
{
	assert(spring);

	// New body's prev is NULL
	spring->prev = NULL;
	// New body's next is the current head of the list
	spring->next = btSprings;
	// Is list empty, if not update prev pointer of current head
	if (btSpringCount > 0)
	{
		// Set the bodies head to the new body's prev
		btSprings->prev = spring->prev;
	}
	//Set head of elements to new element
	btSprings = spring;
	// Increments bodyCount, after adding the body.
	btSpringCount++;
}

void DestroySpring(btSpring_t* spring)
{
	// Assert that the given body is not NULL
	assert(spring);
	// Update the next of the prev body if it exists
	if (spring->prev) spring->prev->next = spring->next;
	// Update the prev of the next body if it exists
	if (spring->next) spring->next->prev = spring->prev;

	if (btSprings == spring)
	{
		btSprings = spring->next;
	}

	btSpringCount--;

	free(spring);

}

void ApplySpringForce(btSpring_t* spring)
{
	for (btSpring_t* spring = btSprings; spring; spring = spring->next)
	{
		Vector2 direction = Vector2Subtract(spring->body2->position, spring->body1->position);
			if (direction.x == 0 && direction.y == 0) continue;

			float length = Vector2Length(direction);
			float x = length - spring->restLength;
			float force = x * spring->k; // -kx <- Hooke's Law

			Vector2 ndirection = Vector2Normalize(direction);

		ApplyForce(spring->body1, Vector2Scale(ndirection, force), FM_FORCE);
		ApplyForce(spring->body2, Vector2Negate(Vector2Scale(ndirection, force)), FM_FORCE);
	}
}