#pragma once
#include "raylib.h"

typedef struct btEditorData
{
    Vector2 anchor01;

    bool WindowBox000Active;
    float MassSliderValue;
    float DampingSliderValue;
    float GravitySliderValue;
    float RestitutionSliderValue;
    float StiffnessSliderValue;
    bool BodyTypeDropdownEditMode;
    int BodyTypeDropdownActive;
    float GravityScaleValue;
    float GravitationScaleValue;
    float TimeStepValueValue;
    bool SimulateToggleActive;
    bool ResetButtonPressed;
} btEditorData_t;

extern btEditorData_t btEditorData;


extern bool ncEditorActive;
extern bool ncEditorIntersect;

void InitEditor();
void UpdateEditor(Vector2 position);
void DrawEditor(Vector2 position);

struct btBody* GetBodyIntersect(struct btBody* bodies, Vector2 position);
void DrawLineBodyToPosition(struct btBody* body, Vector2 position);