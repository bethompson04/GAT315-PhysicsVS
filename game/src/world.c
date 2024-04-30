#include "world.h"
#include <stdlib.h>
#include <assert.h>

Body* bodies = NULL;
int bodyCount = 0;

Body* CreateBody()
{
	// Allocating memory for a new Body
	Body* body = (Body*)malloc(sizeof(Body));
	// Assertion to check if the allocation actually worked
	assert(body);
	// New body's prev is NULL
	body->prev = NULL;
	// New body's next is the current head of the list
	body->next = bodies;
	// Is list empty, if not update prev pointer of current head
	if (bodyCount > 0)
	{
		// Set the bodies head to the new body's prev
		bodies->prev = body->prev;
	}
	//Set the bodies to the new Body
	bodies = body;
	// Increments bodyCount, after adding the body.
	bodyCount++;
	// Returns the new body
	return body;
}

void DestroyBody(Body* body)
{
	// Assert that the given body is not NULL
	assert(body);
	// Update the next of the prev body if it exists
	if (bodies->prev) bodies->prev->next = body->next;
	// Update the prev of the next body if it exists
	if (bodies->next) bodies->next->prev = body->prev;

	if (bodies == body)
	{
		bodies = body->next;
	}

	bodyCount--;

	free(body);

}