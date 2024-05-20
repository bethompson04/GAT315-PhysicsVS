#include "editor.h"
#include "body.h"
#include "render.h"

#define RAYGUI_IMPLEMENTATION
#include "../../raygui/src/raygui.h"

#define EDITOR_DATA(data) TextFormat("%0.2f", data), &data

bool ncEditorActive = true;
bool ncEditorIntersect = false;
Rectangle editorRect;

btEditorData_t btEditorData;

Vector2 EditorBox = { 864, 49 };
Texture2D cursorTexture;

bool WindowBox000Active = true;
//float SliderBar002Value = 0.0f;
//float SliderBar003Value = 0.0f;
//float SliderBar004Value = 0.0f;

void InitEditor()
{
    GuiLoadStyle("raygui/styles/terminal/style_terminal.rgs");

    Image image = LoadImage("resources/reticle 1.png");
    cursorTexture = LoadTextureFromImage(image);
    UnloadImage(image);

    btEditorData.anchor01 = (Vector2){ 808, 48 };

    btEditorData.WindowBox000Active = true;
    btEditorData.BodyTypeDropdownEditMode = false;
    btEditorData.MassSliderValue = 5.0f;
    btEditorData.DampingSliderValue = 1.0f;
    btEditorData.RestitutionSliderValue = 1.0f;
    btEditorData.GravityScaleValue = 1.0f;
    btEditorData.StiffnessSliderValue = 1.0f;
    btEditorData.BodyTypeDropdownActive = 2;
    btEditorData.GravitationScaleValue = 0.0f;
    btEditorData.GravitySliderValue = -30.0f;
    btEditorData.TimeStepValueValue = 60.0f;
    btEditorData.SimulateToggleActive = true;
    btEditorData.ResetButtonPressed = false;
}

void UpdateEditor(Vector2 position)
{
    //
}

void DrawEditor(Vector2 position)
{

    if (btEditorData.BodyTypeDropdownEditMode) GuiLock();

    if (btEditorData.WindowBox000Active)
    {
        btEditorData.WindowBox000Active = !GuiWindowBox((Rectangle){ 840, 24, 264, 424 }, "Editor");
        GuiSliderBar((Rectangle){ 928, 112, 120, 16 }, "Mass", EDITOR_DATA(btEditorData.MassSliderValue), 0, 100);
        GuiSliderBar((Rectangle){ 928, 136, 120, 16 }, "Damping", EDITOR_DATA(btEditorData.DampingSliderValue), 0, 100);
        GuiSliderBar((Rectangle){ 928, 160, 120, 16 }, "GravityScale", EDITOR_DATA(btEditorData.GravitySliderValue), 0, 100);
        GuiSliderBar((Rectangle){ 928, 208, 120, 16 }, "Restitution", EDITOR_DATA(btEditorData.RestitutionSliderValue), 0, 100);
        GuiSliderBar((Rectangle){ 928, 184, 120, 16 }, "Stiffness", EDITOR_DATA(btEditorData.StiffnessSliderValue), 0, 100);
        GuiGroupBox((Rectangle){ 848, 64, 248, 184 }, "BODY");
        GuiGroupBox((Rectangle){ 848, 264, 248, 120 }, "WORLD");
        GuiSlider((Rectangle){ 928, 296, 120, 16 }, "Gravity", EDITOR_DATA(btEditorData.GravityScaleValue), -100, 100);
        GuiSlider((Rectangle){ 928, 320, 120, 16 }, "Gravitation", EDITOR_DATA(btEditorData.GravitationScaleValue), -100, 100);
        GuiSliderBar((Rectangle){ 928, 344, 120, 16 }, "TimeStep", EDITOR_DATA(btEditorData.TimeStepValueValue), 1, 120);
        btEditorData.ResetButtonPressed = GuiButton((Rectangle){ 864, 408, 96, 24 }, "RESET");
        if (GuiDropdownBox((Rectangle){ 920, 72, 136, 24 }, "STATIC;KINEMATIC;DYNAMIC", & btEditorData.BodyTypeDropdownActive, btEditorData.BodyTypeDropdownEditMode)) btEditorData.BodyTypeDropdownEditMode = !btEditorData.BodyTypeDropdownEditMode;
    }
    GuiToggle((Rectangle){ 984, 408, 96, 24 }, "SIMULATE", & btEditorData.SimulateToggleActive);
    
    GuiUnlock();

    DrawTexture(cursorTexture, (int)position.x, (int)position.y, WHITE);
}

btBody* GetBodyIntersect(btBody* bodies, Vector2 position)
{
    for (btBody* body = bodies; body; body = body->next)
    {
        Vector2 screen = ConvertWorldToScreen(body->position);
        if (CheckCollisionPointCircle(position, screen, ConvertWorldToPixel(body->mass * 0.5f)))
        {
            return body;
        }
    }

    return NULL;
}

void DrawLineBodyToPosition(btBody* body, Vector2 position)
{
    Vector2 screen = ConvertWorldToScreen(body->position);
    DrawLine((int)screen.x, (int)screen.y, (int)position.x - cursorTexture.width / 2, (int)position.y - cursorTexture.height / 2, YELLOW);
}