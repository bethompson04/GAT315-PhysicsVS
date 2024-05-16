#pragma once
#include "raylib.h"

typedef struct btEditorData
{
    Vector2 anchor01;

    bool EditBoxActive;
    bool BodyTypeEditMode;
    int BodyTypeActive;
    float MinMassValue;
    float MaxMassValue;
    float GravityScale;
    float DampingValue;
    float GravitationValue;
    float StiffnessValue;
} btEditorData_t;

extern btEditorData_t btEditorData;


extern bool ncEditorActive;
extern bool ncEditorIntersect;

void InitEditor();
void UpdateEditor(Vector2 position);
void DrawEditor(Vector2 position);

struct btBody* GetBodyIntersect(struct btBody* bodies, Vector2 position);
void DrawLineBodyToPosition(struct btBody* body, Vector2 position);