#include "world.h"
#include "mathf.h"
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

    Body* currentBody;


    // game loop
    while (!WindowShouldClose())
    {
        // Update
        float dt = GetFrameTime();
        float fps = (float)GetFPS();

        Vector2 mousePosition = GetMousePosition();
        if (IsMouseButtonDown (0))
        {
            currentBody = CreateBody();

            currentBody->position = mousePosition;
            currentBody->velocity = CreateVector2(GetRandomFloatValue(-5, 5), GetRandomFloatValue(-5, 5));
        }

        // Render
        BeginDrawing();
        ClearBackground(BLACK);
            // stats
        DrawText(TextFormat("FPS: %.2f (%.2f ms)", fps, 1000/fps), 10, 10, 20, LIME);
        DrawText(TextFormat("FRAME: % .4f", dt), 10, 30, 20, LIME);

        DrawCircle((int)mousePosition.x, (int)mousePosition.y, 10.0, YELLOW);


        // update / draw bodies
        Body* body = bodies;
        while (body) // do while we have a valid pointer, will be NULL at the end of the list
        {
            // update body position
            body->position = Vector2Add(body->position, body->velocity);
            // draw body
            DrawCircleLines((int)body->position.x, (int)body->position.y, 15.0, RED);
            body = body->next; // get next body
        }

        EndDrawing();
    }

    CloseWindow();  // Close window and OpenGL context
    free(bodies);

    return 0;
}

