#include "editor.h"
#include "body.h"
#include "render.h"

#define RAYGUI_IMPLEMENTATION
#include "../../raygui/src/raygui.h"

#define EDITOR_DATA(data) TextFormat("%0.2f", data), &data;

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

    btEditorData.EditBoxActive = true;
    btEditorData.BodyTypeEditMode = false;
    btEditorData.BodyTypeActive = 3;
    btEditorData.MinMassValue = 1.0f;
    btEditorData.MaxMassValue = 2.0f;
    btEditorData.GravityScale = 0.0f;
    btEditorData.DampingValue= 0.0f;
    btEditorData.GravitationValue = 0.0f;
    btEditorData.StiffnessValue = 0.0f;
}

void UpdateEditor(Vector2 position)
{
    //
}

void DrawEditor(Vector2 position)
{

    if (btEditorData.BodyTypeEditMode) GuiLock();

    if (btEditorData.EditBoxActive)
    {
        btEditorData.EditBoxActive = !GuiWindowBox((Rectangle) { btEditorData.anchor01.x + 8, btEditorData.anchor01.y + 0, 264, 472 }, "SAMPLE TEXT");
        GuiGroupBox((Rectangle) { btEditorData.anchor01.x + 32, btEditorData.anchor01.y + 96, 216, 240 }, "BODY");
        GuiSliderBar((Rectangle) { btEditorData.anchor01.x + 120, btEditorData.anchor01.y + 168, 120, 16 }, "Min Mass", NULL, & btEditorData.MinMassValue, 0, 100);
        GuiSliderBar((Rectangle) { btEditorData.anchor01.x + 120, btEditorData.anchor01.y + 200, 120, 16 }, "Max Mass", NULL, & btEditorData.MaxMassValue, 0, 100);
        GuiSliderBar((Rectangle) { btEditorData.anchor01.x + 120, btEditorData.anchor01.y + 264, 120, 16 }, "Gravity Scale", NULL, & btEditorData.GravityScale, 0, 100);
        GuiSliderBar((Rectangle) { btEditorData.anchor01.x + 120, btEditorData.anchor01.y + 232, 120, 16 }, "Damping", NULL, & btEditorData.DampingValue, 0, 100);
        GuiSliderBar((Rectangle) { btEditorData.anchor01.x + 120, btEditorData.anchor01.y + 292, 120, 16 }, "Stiffness", NULL, & btEditorData.StiffnessValue, 0, 100);
        GuiSliderBar((Rectangle) { btEditorData.anchor01.x + 120, btEditorData.anchor01.y + 368, 120, 16 }, "World GravForce", NULL, & btEditorData.GravitationValue, 0, 100);
        if (GuiDropdownBox((Rectangle) { btEditorData.anchor01.x + 80, btEditorData.anchor01.y + 120, 120, 24 }, "STATIC;KINEMATIC;DYNAMIC", & btEditorData.BodyTypeActive, btEditorData.BodyTypeEditMode)) btEditorData.BodyTypeEditMode = !btEditorData.BodyTypeEditMode;
    }

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