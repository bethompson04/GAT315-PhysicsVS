#include "collision.h"
#include "contact.h"
#include "body.h"
#include "mathf.h"
#include "raymath.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

bool Intersect(btBody* body1, btBody* body2)
{
    // Calculate the distance between the positions of the two bodies
    float distance = Vector2Distance(body1->position, body2->position);

    // Calculate the combined radius of the two bodies based on their masses
    float radius = body1->mass + body2->mass;

    // Return true if the distance between the bodies is less than their combined radius, indicating an intersection
    return (distance < radius);
}

void CreateContacts(btBody* bodies, ncContact_t** contacts)
{
    // Iterate over each body in the bodies list
    for (btBody* body1 = bodies; body1; body1 = body1->next)
    {
        // Compare each body with every other body in the list
        for (btBody* body2 = bodies; body2; body2 = body2->next)
        {
            // Skip if the bodies are the same
            if (body1 == body2) continue;

            // Skip if neither body is dynamic
            if (body1->type != BT_DYNAMIC && body2->type != BT_DYNAMIC) continue;

            // If the bodies intersect, generate a contact and add it to the contacts list
            if (Intersect(body1, body2))
            {
                ncContact_t* contact = GenerateContact(body1, body2);
                AddContact(contact, contacts);
            }
        }
    }
}

ncContact_t* GenerateContact(btBody* body1, btBody* body2)
{
    // Allocate memory for the contact
    ncContact_t* contact = (ncContact_t*)malloc(sizeof(ncContact_t));
    assert(contact); // Ensure memory allocation was successful

    // Initialize the contact data structure with zeros
    memset(contact, 0, sizeof(ncContact_t));

    // Assign the bodies involved in the contact
    contact->body1 = body1;
    contact->body2 = body2;

    // Calculate the direction vector pointing from body2 to body1
    Vector2 direction = Vector2Subtract(body1->position, body2->position);

    // Calculate the distance between the bodies
    float distance = Vector2Length(direction);

    // If the bodies are at the same position, generate a random direction to avoid division by zero
    if (distance == 0)
    {
        direction = (Vector2){ GetRandomFloatValue(-0.05f, 0.05f), GetRandomFloatValue(-0.05f, 0.05f) };
    }

    // Calculate the effective radius of the bodies
    float radius = (body1->mass + body2->mass);

    // Calculate the penetration depth of the contact
    contact->depth = radius - distance;

    // Calculate the contact normal (direction vector normalized)
    contact->normal = Vector2Normalize(direction);

    // Calculate the restitution coefficient of the contact
    contact->restitution = (body1->restitution + body2->restitution) * 0.5f;

    // Return the generated contact
    return contact;
}


void SeparateContacts(ncContact_t* contacts)
{
	// Iterate over each contact in the contacts list
	for (ncContact_t* contact = contacts; contact; contact = contact->next)
	{
		// Calculate the total inverse mass of the bodies involved in the contact
		float totalInverseMass = contact->body1->inverseMass + contact->body2->inverseMass;

		// Calculate the separation vector based on the contact normal and depth
		Vector2 separation = Vector2Scale(contact->normal, (contact->depth / totalInverseMass));

		// Move the first body based on its inverse mass and separation
		contact->body1->position = Vector2Add(contact->body1->position, Vector2Scale(separation, contact->body1->inverseMass));

		// Move the second body based on its inverse mass and separation (with opposite direction)
		contact->body2->position = Vector2Add(contact->body2->position, Vector2Scale(separation, -contact->body2->inverseMass));
	}
}

void ResolveContacts(ncContact_t* contacts)
{
	// Iterate over each contact in the contacts list
	for (ncContact_t* contact = contacts; contact; contact = contact->next)
	{
		// Calculate the relative velocity of the two bodies in the contact
		Vector2 rv = Vector2Subtract(contact->body1->velocity, contact->body2->velocity);

		// Calculate the normal velocity along the contact normal
		float nv = Vector2DotProduct(rv, contact->normal);

		// If the dot product is greater than 0 / head on
		if (nv > 0) continue;

		// Calculate the total inverse mass of the bodies involved in the contact
		float totalInverseMass = contact->body1->inverseMass + contact->body2->inverseMass;

		// Calculate the impulse magnitude
		float im = -(1 + contact->restitution) * nv / totalInverseMass;

		// Calculate the impulse vector
		Vector2 impulse = Vector2Scale(contact->normal, im);

		// Apply the impulse to the first body
		ApplyForce(contact->body1, impulse, FM_IMPULSE);

		// Apply the opposite impulse to the second body
		ApplyForce(contact->body2, Vector2Negate(impulse), FM_IMPULSE);
	}
}
