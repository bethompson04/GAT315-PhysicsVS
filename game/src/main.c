#include "world.h"
#include "mathf.h"
#include "Integrator.h"
#include "raylib.h"
#include "raymath.h"

#include <stdlib.h>
#include <assert.h>

#define MAX_BODIES 100

//----------------------------------------------------------------------------------
// Main entry point
//----------------------------------------------------------------------------------
int main(void)
{
    InitWindow(1280, 720, "Phsyics engin");
    SetTargetFPS(60);

    // Initialize World
    btGravity = (Vector2){ 0, 30 };

    btBody* currentBody;
    btBody* body;

    // game loop
    while (!WindowShouldClose())
    {
        // Update
        float dt = GetFrameTime();
        float fps = (float)GetFPS();

        Vector2 mousePosition = GetMousePosition();
        if (IsMouseButtonDown(0))
        {
            currentBody = CreateBody();
            currentBody->position = mousePosition;
            currentBody->mass = GetRandomFloatValue(1, 5);
            currentBody->inverseMass = 1 / currentBody->mass;
            currentBody->damping = 0.2f;
            currentBody->gravityScale = 20;
            currentBody->type = BT_DYNAMIC;
            ApplyForce(currentBody, (Vector2) { GetRandomFloatValue(-100, 100), GetRandomFloatValue(-100, 100)}, FM_VELOCITY);
        }

        // Apply Force
        body = btBodies;
        while (body) // do while there is a valid pointer, will be NULL at the end of the list
        {
            // update body position
            //ApplyForce(body, CreateVector2(0, -100), FM_FORCE);
            body = body->next;
        }

        // Update Bodies
        body = btBodies;
        while (body) // do while there is a valid pointer, will be NULL at the end of the list
        {
            // update body position
            Step(body, dt);
            body = body->next; 
        }

        // Render
        BeginDrawing();
        ClearBackground(BLACK);
            // stats
        DrawText(TextFormat("FPS: %.2f (%.2f ms)", fps, 1000/fps), 10, 10, 20, LIME);
        DrawText(TextFormat("FRAME: % .4f", dt), 10, 30, 20, LIME);

        DrawCircle((int)mousePosition.x, (int)mousePosition.y, 10.0, YELLOW);


        // draw bodies
        body = btBodies;
        while (body) // do while we have a valid pointer, will be NULL at the end of the list
        {
            // draw body
            DrawCircleLines((int)body->position.x, (int)body->position.y, body->mass, RED);
            body = body->next; // get next body
        }

        EndDrawing();
    }

    CloseWindow();  // Close window and OpenGL context
    free(btBodies);

    return 0;
}

