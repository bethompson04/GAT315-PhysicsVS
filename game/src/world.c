#include "world.h"
#include "body.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

btBody* btBodies = NULL;
int btBodyCount = 0;
Vector2 btGravity;

btBody* CreateBody()
{
	// Allocating memory for a new Body
	btBody* body = (btBody*)malloc(sizeof(btBody));
	// Assertion to check if the allocation actually worked
	assert(body);

	// Clearing the memory of the body pointer
	memset(body, 0, sizeof(btBody));

	// New body's prev is NULL
	body->prev = NULL;
	// New body's next is the current head of the list
	body->next = btBodies;
	// Is list empty, if not update prev pointer of current head
	if (btBodyCount > 0)
	{
		// Set the bodies head to the new body's prev
		btBodies->prev = body->prev;
	}
	//Set the bodies to the new Body
	btBodies = body;
	// Increments bodyCount, after adding the body.
	btBodyCount++;
	// Returns the new body
	return body;
}

void DestroyBody(btBody* body)
{
	// Assert that the given body is not NULL
	assert(body);
	// Update the next of the prev body if it exists
	if (btBodies->prev) btBodies->prev->next = body->next;
	// Update the prev of the next body if it exists
	if (btBodies->next) btBodies->next->prev = body->prev;

	if (btBodies == body)
	{
		btBodies = body->next;
	}

	btBodyCount--;

	free(body);

}