#include <iostream>
#include "raylib.h"
#include "raymath.h"

// chunk dims
#define CX 9
#define CY 15
#define CZ 9

int main(){

    //Vars
    const int win_x = 800;
    const int win_y = 450;
    Vector3 spawnPos = {4.0f, 9.0f, 4.0f};
    const Color wood   = {139, 90,  43,  255};
    const Color cobble = {128, 128, 128, 255};
    const Color grass  = { 0, 117, 44, 255 };
    const Color dirt   = {134, 96,  67,  255};
    int selection = 1;

    //Window
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(win_x, win_y, "Block Game");
    SetTargetFPS(60);

    //Camera
    Camera3D camera = {0};
    camera.fovy = 60.0f;
    camera.position = spawnPos;
    camera.target = (Vector3){5.0f, 9.0f, 5.0f};
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    camera.projection = CAMERA_PERSPECTIVE;

    // Blocks: 0=air, 1=grass, 2=dirt, 3=wood, 4=cobble
    int blocks[CX][CY][CZ];
    for(int x = 0; x < CX; x++){
        for(int y = 0; y < CY; y++){
            for(int z = 0; z < CZ; z++){
                if(y == 0 || y == 1)      blocks[x][y][z] = 4; // cobble bottom
                else if(y >= 2 && y <= 4) blocks[x][y][z] = 2; // dirt middle
                else if(y == 5)           blocks[x][y][z] = 1; // grass top
                else                      blocks[x][y][z] = 0; // air above
            }
        }
    }

    DisableCursor();

    // Physics
    float velY = 0.0f;
    const float gravity = -20.0f;
    const float jumpForce = 8.0f;
    const float playerHeight = 1.8f;
    bool onGround = false;

    //Game Loop
    while(!WindowShouldClose()){

        float dt = GetFrameTime();

        if(IsKeyPressed(KEY_F11)){
            ToggleFullscreen();
            camera.fovy = 100.0f;
        } 
        if(IsKeyPressed(KEY_ONE))   selection = 1;
        if(IsKeyPressed(KEY_TWO))   selection = 2;
        if(IsKeyPressed(KEY_THREE)) selection = 3;
        if(IsKeyPressed(KEY_FOUR))  selection = 4;

        int cx = GetScreenWidth()/2;
        int cy = GetScreenHeight()/2;
        int size = 10;

        // ── GRAVITY & JUMPING ──────────────────────────────────────
        velY += gravity * dt;
        camera.position.y += velY * dt;
        camera.target.y   += velY * dt;

        // floor collision — check block below player feet
        float feetY = camera.position.y - playerHeight;
        int px = (int)roundf(camera.position.x);
        int pz = (int)roundf(camera.position.z);
        int floorBlock = (int)roundf(feetY);

        // check if there's a solid block right under feet
        if(px >= 0 && px < CX && pz >= 0 && pz < CZ){
            for(int y = 0; y < CY; y++){
                float blockTop = y + 0.5f;
                if(blocks[px][y][pz] != 0 &&
                   camera.position.y - playerHeight <= blockTop &&
                   velY < 0){
                    // land on block
                    float landY = blockTop + playerHeight;
                    camera.target.y += (landY - camera.position.y);
                    camera.position.y = landY;
                    velY = 0.0f;
                    onGround = true;
                    break;
                }
            }
        }

        // check if still on ground this frame
        onGround = false;
        if(px >= 0 && px < CX && pz >= 0 && pz < CZ){
            int checkY = (int)roundf(camera.position.y - playerHeight - 0.1f);
            if(checkY >= 0 && checkY < CY && blocks[px][checkY][pz] != 0)
                onGround = true;
        }

        // jump
        if(IsKeyPressed(KEY_SPACE) && onGround){
            velY = jumpForce;
        }

        // world floor safety net
        if(camera.position.y < -10.0f){
            camera.position = spawnPos;
            camera.target = (Vector3){5.0f, 9.0f, 5.0f};
            velY = 0.0f;
        }

        // ── CAMERA (horizontal movement only) ─────────────────────
        UpdateCamera(&camera, CAMERA_FIRST_PERSON);

        // ── RAYCASTING ────────────────────────────────────────────
        Ray ray = GetMouseRay({(float)cx, (float)cy}, camera);
        RayCollision closestHit = {0};
        closestHit.distance = 99999.0f;
        int bx = -1, by = -1, bz = -1;

        for(int x = 0; x < CX; x++){
            for(int y = 0; y < CY; y++){
                for(int z = 0; z < CZ; z++){
                    if(blocks[x][y][z] == 0) continue;
                    BoundingBox box = {
                        (Vector3){x - 0.5f, y - 0.5f, z - 0.5f},
                        (Vector3){x + 0.5f, y + 0.5f, z + 0.5f}
                    };
                    RayCollision hit = GetRayCollisionBox(ray, box);
                    if(hit.hit && hit.distance < closestHit.distance){
                        closestHit = hit;
                        bx = x; by = y; bz = z;
                    }
                }
            }
        }

        // ── BREAK & PLACE ─────────────────────────────────────────
        if(closestHit.hit && closestHit.distance < 5.0f){
            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                blocks[bx][by][bz] = 0;
            if(IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)){
                int nx = bx + (int)closestHit.normal.x;
                int ny = by + (int)closestHit.normal.y;
                int nz = bz + (int)closestHit.normal.z;
                if(nx>=0 && nx<CX && ny>=0 && ny<CY && nz>=0 && nz<CZ)
                    blocks[nx][ny][nz] = selection;
            }
        }

        // ── DRAW ──────────────────────────────────────────────────
        BeginDrawing();
        ClearBackground(SKYBLUE);
        BeginMode3D(camera);

        for(int x = 0; x < CX; x++){
            for(int y = 0; y < CY; y++){
                for(int z = 0; z < CZ; z++){
                    if(blocks[x][y][z] == 0) continue;
                    Color block_cl = grass;
                    if(blocks[x][y][z] == 2) block_cl = dirt;
                    if(blocks[x][y][z] == 3) block_cl = wood;
                    if(blocks[x][y][z] == 4) block_cl = cobble;
                    DrawCube({(float)x,(float)y,(float)z}, 1.0f, 1.0f, 1.0f, block_cl);
                    DrawCubeWires({(float)x,(float)y,(float)z}, 1.0f, 1.0f, 1.0f, BLACK);
                }
            }
        }

        // highlight looked at block
        if(closestHit.hit && closestHit.distance < 5.0f)
            DrawCubeWires({(float)bx,(float)by,(float)bz}, 1.02f, 1.02f, 1.02f, YELLOW);

        DrawGrid(30, 1.0f);
        EndMode3D();

        // crosshair
        DrawLine(cx - size, cy, cx + size, cy, WHITE);
        DrawLine(cx, cy - size, cx, cy + size, WHITE);

        // hotbar UI
        DrawText(TextFormat("Selected: %d", selection), 10, 40, 20, WHITE);
        DrawText("[1]Grass [2]Dirt [3]Wood [4]Cobble", 10, 65, 16, LIGHTGRAY);

        DrawFPS(10, 10);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}