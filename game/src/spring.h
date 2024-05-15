#pragma once

typedef struct btSpring
{
	struct btBody* body1;
	struct btBody* body2;
	float restLength;
	float k; // stiffness / love force

	struct btSpring* next;
	struct btSpring* prev;

}btSpring_t;

extern btSpring_t* btSprings;

btSpring_t* CreateSpring(struct btBody* body1, struct btBody* body2, float restLength, float k);
void AddSpring(btSpring_t* spring);
void DestroySpring(btSpring_t* spring);
void DestroyAllSprings();

void ApplySpringForce(btSpring_t* spring);