#include "Integrator.h"
#include "spring.h"
#include "Force.h"
#include "world.h"
#include "render.h"
#include "editor.h"
#include "collision.h"
#include "contact.h"
#include "raylib.h"

#include <stdlib.h>
#include <assert.h>

#define MAX_BODIES 100

//----------------------------------------------------------------------------------
// Main entry point
//----------------------------------------------------------------------------------

int main(void)
{
    btBody* selectedBody = NULL;
    btBody* connectBody = NULL;

    InitWindow(1280, 720, "Phsyics engin");
    InitEditor();
    SetTargetFPS(60);

    // Initialize World
    btGravity = (Vector2){ 0, 0 };

    btBody* currentBody;
    btBody* body;

    int mode = 0;

    // game loop
    while (!WindowShouldClose())
    {
        // Update
        float dt = GetFrameTime();
        float fps = (float)GetFPS();
        if (IsKeyPressed(KEY_TAB))
        {
            if (mode < 3) mode++;
            else mode = 0;
        }
        Vector2 mousePosition = GetMousePosition();
        ncScreenZoom += GetMouseWheelMove() * 0.2f;
        ncScreenZoom = Clamp(ncScreenZoom, 0.1f, 10);


        UpdateEditor(mousePosition);

        selectedBody = GetBodyIntersect(btBodies, mousePosition);
        if (selectedBody)
        {
            Vector2 screen = ConvertWorldToScreen(selectedBody->position);
            DrawCircleLines(screen.x, screen.y, ConvertWorldToPixel(selectedBody->mass * 0.5f) + 5, YELLOW);
        }

        // Create Body
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            switch (mode)
            {
            case 0:
                currentBody = CreateBody(ConvertScreenToWorld(mousePosition), btEditorData.MinMassValue, btEditorData.BodyTypeActive);

                currentBody->damping = 0.0f;
                currentBody->gravityScale = 20;
                currentBody->color = (Color){ 255, 0 , 0, 255 };

                AddBody(currentBody);

                //ApplyForce(currentBody, (Vector2) { GetRandomFloatValue(-5, 5), GetRandomFloatValue(-5, 5) }, FM_VELOCITY);
                break;
            case 1:
                for (int i = 0; i < 10; i++)
                {
                    currentBody = CreateBody(ConvertScreenToWorld(mousePosition), btEditorData.MinMassValue, btEditorData.BodyTypeActive);
 
                    currentBody->damping = 0.9f;
                    currentBody->gravityScale = 10;
                    currentBody->color = (Color){ GetRandomFloatValue(0, 255),  GetRandomFloatValue(0, 255),  GetRandomFloatValue(0, 255), 255 };

                    AddBody(currentBody);

                    //ApplyForce(currentBody, (Vector2) { GetRandomFloatValue(-250, 250), GetRandomFloatValue(-150, -500)}, FM_VELOCITY);
                }
                break;
            case 2:
                for (int i = 0; i < 15; i++)
                {
                    currentBody = CreateBody(ConvertScreenToWorld(mousePosition), btEditorData.MinMassValue, btEditorData.BodyTypeActive);

                    currentBody->damping = 0.80f;
                    currentBody->gravityScale = 5;
                    currentBody->color = (Color){ GetRandomFloatValue(0, 255),  GetRandomFloatValue(0, 255),  GetRandomFloatValue(0, 255), 255 };

                    AddBody(currentBody);

                    //Vector2 force = Vector2Scale(GetVector2FromAngle((360 + GetRandomFloatValue(-30, 30))), GetRandomFloatValue(100, 1000));
                    //ApplyForce(currentBody, force, FM_IMPULSE);
                }
                break;
            case 3:
                for (int i = 0; i < 2; i++)
                {
                    currentBody = CreateBody(ConvertScreenToWorld(mousePosition), btEditorData.MinMassValue, btEditorData.BodyTypeActive);

                    currentBody->damping = 0.80f;
                    currentBody->gravityScale = 5;
                    currentBody->color = (Color){ 255, 255, 0, 255 };

                    AddBody(currentBody);

                    float angle = 240;
                    if (i == 0) angle = 300;

                    //Vector2 force = Vector2Scale(GetVector2FromAngle(angle * DEG2RAD), GetRandomFloatValue(1000, 2500));
                    //ApplyForce(currentBody, force, FM_IMPULSE);
                }
                break;
            }

        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && selectedBody)
        {
            connectBody = selectedBody;
        }
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && connectBody) DrawLineBodyToPosition(connectBody, mousePosition);
        if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT) && connectBody)
        {
            if (selectedBody && connectBody != selectedBody)
            {
                btSpring_t* spring = CreateSpring(connectBody, selectedBody, Vector2Distance(connectBody->position, selectedBody->position), 20);
                AddSpring(spring);
            }
        }

        // Apply Force
        ApplyGravitation(btBodies, btEditorData.GravitationValue);
        ApplySpringForce(btSprings);

        // Update Bodies
        for (btBody* body = btBodies; body; body = body->next)
        {
            Step(body, dt);
        }

        // collision
        ncContact_t* contacts = NULL;
        CreateContacts(btBodies, &contacts);

        // Render
        BeginDrawing();
        ClearBackground(BLACK);


        // stats
        DrawText(TextFormat("FPS: %.2f (%.2f ms)", fps, 1000/fps), 10, 10, 20, LIME);
        DrawText(TextFormat("FRAME: % .4f", dt), 10, 30, 20, LIME);
        DrawText(TextFormat("Emission Type: (%i)", mode),10, 50, 20, LIME);

        //DrawCircle((int)mousePosition.x, (int)mousePosition.y, 10.0, YELLOW);


        // draw bodies
        for (btBody* body = btBodies; body; body = body->next)
        {
            Vector2 screen = ConvertWorldToScreen(body->position);
            if (mode == 1) DrawRectangle((int)screen.x, (int)screen.y, (int)ConvertWorldToPixel(body->mass), (int)body->mass, body->color);
            else DrawCircle((int)screen.x, (int)screen.y, ConvertWorldToPixel(body->mass * 0.5f), body->color);
            Step(body, dt);
        }

        // Draw Springs
        for (btSpring_t* spring = btSprings; spring; spring = spring->next)
        {
            Vector2 screen1 = ConvertWorldToScreen(spring->body1->position);
            Vector2 screen2 = ConvertWorldToScreen(spring->body2->position);

            DrawLine((int)screen1.x, (int)screen1.y, (int)screen2.x, (int)screen2.y, YELLOW);
        }

        // Draw Contacts
        for (ncContact_t* contact = contacts; contact; contact = contact->next)
        {
            Vector2 screen = ConvertWorldToScreen(contact->body1->position);

            DrawCircle((int)screen.x, (int)screen.y, ConvertWorldToPixel(contact->body1->mass * 0.5f), YELLOW);
        }

        DrawEditor(mousePosition);

        EndDrawing();
    }

    CloseWindow();  // Close window and OpenGL context
    free(btBodies);

    return 0;
}



