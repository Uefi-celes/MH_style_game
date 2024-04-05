#include <raylib.h>
#include <math.h>
float screen360 = 16/3;
float rotation = 0;
//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1920;
    const int screenHeight = 1080;
    float cube_pos_x = 2.0;
    float cube_pos_y = 2.0;
    float cube_pos_z = 2.0;
    float rotation = 0.0f;
    InitWindow(screenWidth, screenHeight, "raylib [core] example - 3d camera free");

    const Model PLAYER = LoadModel("./Models/T.fbx");

    Camera3D camera = { 0 };
    camera.position = (Vector3){ cube_pos_x + 10.0f, cube_pos_y + 10.0f, 10.0f }; // Camera position
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.position = (Vector3){ cube_pos_x + 10.0f, 10.0f, cube_pos_z + 10.0f}; // Camera position
    int cameraX = cube_pos_x + 10.0f;
    int cameraZ = cube_pos_z + 10.0f;
    camera.target = (Vector3){ cube_pos_x, cube_pos_y, cube_pos_z };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };

    int mousex = GetMouseX();
    int mousey = GetMouseY();

    DisableCursor();
    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        mousey = GetMouseY();
        int Mouse_Position = GetMouseX();
        if(Mouse_Position >= 1920) {SetMousePosition(0, GetMouseY());}
        Vector3 cubePosition = { cube_pos_x, cube_pos_y, cube_pos_z };
            UpdateCamera(&camera.position, CAMERA_THIRD_PERSON);
            camera.target = (Vector3){ cube_pos_x, cube_pos_y, cube_pos_z };      // Camera looking at poin

            BeginDrawing();

            ClearBackground(RAYWHITE);
            DrawModel(PLAYER, cubePosition, 10.0f, MAROON);

            DrawModelEx(PLAYER, cubePosition, cubePosition, rotation, (Vector3){1.0f, 1.0f, 1.0f}, MAROON);

            BeginMode3D(camera);
            if(IsKeyDown(KEY_W) || IsKeyDown(KEY_A) || IsKeyDown(KEY_S) || IsKeyDown(KEY_D))
            {
                //move the map
                if(IsKeyDown(KEY_W)) {cube_pos_z -= 1; camera.position.z -= 1;}
                if(IsKeyDown(KEY_S)) {cube_pos_z += 1; camera.position.z += 1;}
                if(IsKeyDown(KEY_A)) {cube_pos_x -= 1; camera.position.x -= 1;}
                if(IsKeyDown(KEY_D)) {cube_pos_x += 1; camera.position.x += 1;}
            }
            else
            {
                //move the camera
                if(IsKeyDown(KEY_W)) {cube_pos_z -= 1; camera.position.z -= 1;}
                if(IsKeyDown(KEY_S)) {cube_pos_z += 1; camera.position.z += 1;}
                if(IsKeyDown(KEY_A)) {cube_pos_x -= 1; camera.position.x -= 1;}
                if(IsKeyDown(KEY_D)) {cube_pos_x += 1; camera.position.x += 1;}
            }

                DrawCircle3D(cubePosition, 10.0f, cubePosition, rotation, RED);
                DrawCubeWires(cubePosition, 2.0f, 2.0f, 2.0f, MAROON);

                DrawGrid(100, 1.0f);

            EndMode3D();

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
