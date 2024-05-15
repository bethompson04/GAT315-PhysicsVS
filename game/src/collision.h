#pragma once
#include "raylib.h"

typedef struct ncContact ncContact_t;
typedef struct btBody btBody;

void CreateContacts(btBody* bodies, ncContact_t** contacts);
ncContact_t* GenerateContact(btBody* body1, btBody* body2);

void SeparateContacts(ncContact_t* contacts);
void ResolveContacts(ncContact_t* contacts);