#include "raylib.h"
#include "raymath.h"

#include "math.h"
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "pthread.h"

int zero;

float rotation_cos;
float hit_radius = 50.0f;
float char_position_x;
float char_position_z;

int possible_fps[11] = {30, 60, 90, 120, 144, 180, 210, 240, 440, 560, 40};
int i = 4;
int target_fps;
int fps_seconds = 0;

int player_color_counter = 1;

bool jump = false;
float Jump_Factor;
float Future_Position_x; // the future position is the position that the player should go to, wether that be opposite of the camera or to the camera
float Current_Position_x; // the current position of the character
float Position_Squarred_x; // needs to be renamed but its the future position subtracted the current position

float Future_Position_z;
float Current_Position_z;
float Position_Squarred_z;

int Dashing_Timer = 0;
int dash_timer = 0;
bool dash_timer_bool;

float Square_Jump_hold;

Vector3 Z_Axis_Start = {0, 0, 100}; // Variables to make the x and z zxis have color
Vector3 Z_Axis_End = {0, 0, -100};

Vector3 X_Axis_Start = {100, 0, 0};
Vector3 X_Axis_End = {-100, 0, 0};

Vector2 Box = {10, 10};

Vector2 MousePos;

int camera_distance = 4;

Vector2 Pthag;
float Pthag_Z;

float MouseXPercent;
float MouseXChange;
float MouseZChangeSin;
float MouseXChangeCos;
float mouseX;

float MouseYPercent;
float mouseY;

float MouseTan;

float cam_player_percent_x;
float cam_player_percent_y;

float UPndDOWN = 1.0;
float UPndDOWNSIN;
float UPndDOWNCOS;
Vector2 screen_size = {1920, 1080};
float speed = 1;

int attack_time = 10;

bool timer = false;

Vector3 Player_model;
bool title_screen = true;
bool character_select = false;
bool combat = false;
int collision = 0;

bool Dashing = false;

Vector3 Box_3d = {0.5f,0.5f,0.5f};

float hp_percentage;

Vector3 scale = {0.01f,0.01f,0.01f};
Vector3 Turret_scale = {1.0f, 1.0f, 1.0f};
Vector3 rotation = {0,1,0};

typedef struct
{
    Vector3 Player;
    Vector3 Player_hit_box_size;
    int player_color_counter;
    int Max_Health;
    int Health_points;
    float speed_variable;
    float Max_Dash;
    float Dash;
    float Jump_Factor;
    float Bullet_Range;
    Model Player_Character;
    Mesh Character_Mesh;
    BoundingBox Player_hit_box;


}player;

player players[11];

int your_player = 0;
int Active_Players = 0;


typedef struct
{
    Vector3 Bullet_Speed;
    Vector3 Bullet_Start;
    Vector3 Bullet_Position;
    Vector3 Bullet_FPosition;
    Model Bullet_Model;
    bool death;
}bullet;
bullet bullet_string[10000];
long int Bullet_Death = 0;
long int Bullet_Number = 0;
int fps_bullets;
int Bullet_Range;

typedef struct
{
    int Enemy_Health;
    int Health_points;
    Vector3 enemy_pos;
    Vector3 enemy_pos_upper;
    Model turret;
    float enemy_rotation;
    float length;
    bool Turret_summon;
    bullet Turret_Bullet[10000];
    long int Bullet_Death;
    int timer;
    bool timer_bool;
    int players_that_can_be_hit;
    int player_in_range[10];
    int player_range[10];
}Enemy_Creation;

int Spawnable_Turrets = 3;
Enemy_Creation Enemy_Creat[3];

int Turret_Number = 0;
int hold;

int IP_ADDRESS[12];
int IP_PLACE = 0;
int octet_1 = 0;
int octet_2 = 0;
int octet_3 = 0;
int octet_4 = 0;

int port_number;


int main()
{
    int cpu_count = sysconf(_SC_NPROCESSORS_ONLN);

    Rectangle six_nine = {5, 105, 590, 90};
    Rectangle ninetwenty_teneight = {5, 205, 590, 90};
    Rectangle twok = {5, 305, 590, 90};
    Rectangle fourk = {5, 405, 590, 90};
    InitWindow(600,600,"resolution");
    while(Dashing == false && !WindowShouldClose())
    {

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawRectangleRec(six_nine, GRAY);
        int ninetwenty = 1920;
        DrawRectangleRec(ninetwenty_teneight, GRAY);
        DrawRectangleRec(twok, GRAY);
        DrawRectangleRec(fourk, GRAY);
        MousePos = GetMousePosition();
        Rectangle Mouse_Box = {MousePos.x - 10.0f, MousePos.y - 10.0f, 20, 20};


        DrawText(TextFormat("%i",1280), 10, 105, 30, GREEN);
        DrawText(TextFormat("x"), 75, 105, 30, GREEN);
        DrawText(TextFormat("%i",720), 95, 105, 30, GREEN);

        DrawText(TextFormat("%i",1920), 10, 205, 30, GREEN);
        DrawText(TextFormat("x"), 75, 205, 30, GREEN);
        DrawText(TextFormat("%i",1080), 95, 205, 30, GREEN);

        DrawText(TextFormat("%i",2560), 10, 305, 30, GREEN);
        DrawText(TextFormat("x"), 85, 305, 30, GREEN);
        DrawText(TextFormat("%i",1440), 105, 305, 30, GREEN);

        DrawText(TextFormat("%i",4096), 10, 405, 30, GREEN);
        DrawText(TextFormat("x"), 85, 405, 30, GREEN);
        DrawText(TextFormat("%i",2160), 105, 405, 30, GREEN);

        if(CheckCollisionRecs(six_nine, Mouse_Box) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            screen_size.x = 1280;
            screen_size.y = 720;
            Dashing = true;
        }

        if(CheckCollisionRecs(ninetwenty_teneight, Mouse_Box) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            screen_size.x = 1920;
            screen_size.y = 1080;
            Dashing = true;
        }

        if(CheckCollisionRecs(twok, Mouse_Box) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            screen_size.x = 2560;
            screen_size.y = 1440;
            Dashing = true;
        }

        if(CheckCollisionRecs(fourk, Mouse_Box) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            screen_size.x = 4096;
            screen_size.y = 2160;
            Dashing = true;
        }

        EndDrawing();
    }
    Dashing = false;
    target_fps = possible_fps[i];

    InitWindow(screen_size.x, screen_size.y, "test");

    float scale_factor = screen_size.x / 1920;

    printf("initialize");

    Vector2 half_screen = {screen_size.x / 2, screen_size.y / 2};
    Model BoonBoomChanger = LoadModel("./Game_Models/BOOMAXE/BoonBoomBooster.gltf");
    Model Turret_Top = LoadModel("./Game_Models/TURRET/Turret_Top.glb");
    Model Turret_Bottom = LoadModel("./Game_Models/TURRET/Turret_Bottom.glb");

    if(IsModelValid(Turret_Top) == false)
    {
        Turret_Top = LoadModelFromMesh(GenMeshCube(1.0f, 1.0f, 2.0f));
    }
    if(IsModelValid(Turret_Bottom) == false)
    {
        Turret_Bottom = LoadModelFromMesh(GenMeshCube(1.0f, 2.0f,1.0f));
    }
    if(IsModelValid(BoonBoomChanger) == false)
    {
        BoonBoomChanger = LoadModelFromMesh(GenMeshCube(100.5f, 500.0f,100.5f));
    }
    for(Turret_Number = 0; Turret_Number < Spawnable_Turrets;Turret_Number ++)
    {
        Enemy_Creat[Turret_Number].Turret_summon = false;
        Enemy_Creat[Turret_Number].enemy_pos = (Vector3){ -9999.9f, -9999.9f, -9999.9f};
        Enemy_Creat[Turret_Number].players_that_can_be_hit = 0;
        Enemy_Creat[Turret_Number].turret = Turret_Top;
    }

    Camera3D camera = { 0 };

    camera.position = (Vector3){10.0f, 2.50f, 10.0f };
    camera.target = (Vector3){0, 0, 0};
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_CUSTOM;

    Camera3D Camera_Unbound;
    Camera_Unbound.position = camera.position;
    Camera_Unbound.target = camera.target;
    Camera_Unbound.up = camera.up;
    Camera_Unbound.fovy = camera.fovy;
    Camera_Unbound.projection = CAMERA_CUSTOM;

    int Current_turret = 0;
    void Turret_Select(void)
    {
        Enemy_Creat[Current_turret].Health_points = 250;
        Enemy_Creat[Current_turret].Turret_summon = true;
        Enemy_Creat[Current_turret].enemy_rotation = (atan2(Camera_Unbound.position.x, Camera_Unbound.position.z) * 180)/ M_PI;
        Enemy_Creat[Current_turret].enemy_pos = (Vector3){players[your_player].Player.x, 1.5f, players[your_player].Player.z};
        Current_turret += 1;
        if(Current_turret == Spawnable_Turrets)
        {
            Current_turret = 0;
        }
    }
    void enemy_ai_bullet(void)
    {
        if(round(rotation_cos) - (360 - Enemy_Creat[Turret_Number].enemy_rotation) < 1.0f && round(rotation_cos) - (360 - Enemy_Creat[Turret_Number].enemy_rotation) > -1.0f && Enemy_Creat[Turret_Number].timer_bool == false
        && CheckCollisionSpheres(players[Enemy_Creat[Turret_Number].players_that_can_be_hit].Player, 0.5f, Enemy_Creat[Turret_Number].enemy_pos, 50.0f))
        {

            Enemy_Creat[Turret_Number].Turret_Bullet[Enemy_Creat[Turret_Number].Bullet_Death].death = false;
            Enemy_Creat[Turret_Number].Turret_Bullet[Enemy_Creat[Turret_Number].Bullet_Death].Bullet_Position = Enemy_Creat[Turret_Number].enemy_pos;
            Enemy_Creat[Turret_Number].timer_bool = true;

            Enemy_Creat[Turret_Number].Turret_Bullet[Enemy_Creat[Turret_Number].Bullet_Death].Bullet_Speed = (Vector3)
            {
             sin((Enemy_Creat[Turret_Number].enemy_rotation -90.0f) * M_PI/180 ) / GetFPS() * 50,
             -sin(Enemy_Creat[Turret_Number].length) / GetFPS() * 50,
             cos((Enemy_Creat[Turret_Number].enemy_rotation -90.0f) * M_PI/180 ) / GetFPS() * 50
            };

            Enemy_Creat[Turret_Number].Bullet_Death += 1;
        }
        if(Enemy_Creat[Turret_Number].timer_bool == true)
        {
            Enemy_Creat[Turret_Number].timer += 1;
        }
        if(Enemy_Creat[Turret_Number].timer >= 100 || Enemy_Creat[Turret_Number].timer < 0)
        {
            Enemy_Creat[Turret_Number].timer = 0;
            Enemy_Creat[Turret_Number].timer_bool = false;
        }
        for(zero = 0; zero < Enemy_Creat[Turret_Number].Bullet_Death; zero++)
        {
            if(Enemy_Creat[Turret_Number].Turret_Bullet[zero].death == false)
            {
                Enemy_Creat[Turret_Number].Turret_Bullet[zero].Bullet_Position.x = Enemy_Creat[Turret_Number].Turret_Bullet[zero].Bullet_Position.x + Enemy_Creat[Turret_Number].Turret_Bullet[zero].Bullet_Speed.x;
                Enemy_Creat[Turret_Number].Turret_Bullet[zero].Bullet_Position.y = Enemy_Creat[Turret_Number].Turret_Bullet[zero].Bullet_Position.y + Enemy_Creat[Turret_Number].Turret_Bullet[zero].Bullet_Speed.y;
                Enemy_Creat[Turret_Number].Turret_Bullet[zero].Bullet_Position.z = Enemy_Creat[Turret_Number].Turret_Bullet[zero].Bullet_Position.z + Enemy_Creat[Turret_Number].Turret_Bullet[zero].Bullet_Speed.z;
                DrawSphere(Enemy_Creat[Turret_Number].Turret_Bullet[zero].Bullet_Position, 0.25f, RED);
                if(sqrt(pow(Enemy_Creat[Turret_Number].enemy_pos.x - Enemy_Creat[Turret_Number].Turret_Bullet[zero].Bullet_Position.x, 2)
                + pow(Enemy_Creat[Turret_Number].enemy_pos.z - Enemy_Creat[Turret_Number].Turret_Bullet[zero].Bullet_Position.z, 2) > 1000.0f))
                {
                    Enemy_Creat[Turret_Number].Turret_Bullet[zero].death = true;
                }
                for(int zero2 = 0; zero2 <= Active_Players; zero2 ++)
                {
                    if(CheckCollisionBoxSphere(players[zero2].Player_hit_box, Enemy_Creat[Turret_Number].Turret_Bullet[zero].Bullet_Position, 0.25f))
                    {
                        players[zero2].Health_points -= 10;
                        Enemy_Creat[Turret_Number].Turret_Bullet[zero].death = true;
                    }
                }

            }
        }
        if(Enemy_Creat[Turret_Number].Health_points <= 0)
        {
            Enemy_Creat[Turret_Number].Turret_summon = false;
            Enemy_Creat[Turret_Number].enemy_pos = (Vector3){ -9999.9f, -9999.9f, -9999.9f};
            for(zero = 0; zero <= Enemy_Creat[Turret_Number].Bullet_Death; zero ++)
            {
                Enemy_Creat[Turret_Number].Turret_Bullet[zero].death = true;
                Enemy_Creat[Turret_Number].timer_bool = false;
                Enemy_Creat[Turret_Number].Bullet_Death = 0;
            }
        }
    }
    void enemy_ai_hitscan(void)
    {
        if(round(rotation_cos) - (360 - Enemy_Creat[Turret_Number].enemy_rotation) < 1.0f && round(rotation_cos) - (360 - Enemy_Creat[Turret_Number].enemy_rotation) > -1.0f && Enemy_Creat[Turret_Number].timer_bool == false)
        {
            players[Enemy_Creat[Turret_Number].players_that_can_be_hit].Health_points -= 100;
        }
    }
    void turret_algorithm(void)
    {

        int zero2 = 0;
        Enemy_Creat[Turret_Number].players_that_can_be_hit = 0;
        for(zero = 0; zero <= Active_Players; zero++)
        {
            Enemy_Creat[Turret_Number].player_in_range[zero2] = zero;
            Enemy_Creat[Turret_Number].player_range[zero2] = sqrt(pow(Enemy_Creat[Turret_Number].enemy_pos.x - players[zero].Player.x, 2) + pow(Enemy_Creat[Turret_Number].enemy_pos.z - players[zero].Player.z , 2));
            zero2++;
            Enemy_Creat[Turret_Number].players_that_can_be_hit ++;

        }
        for(zero = 0, zero2 = 0; zero <= Active_Players; zero++)
        {
            if(Enemy_Creat[Turret_Number].player_range[zero2] > Enemy_Creat[Turret_Number].player_range[zero])
            {
                zero2 = zero;
            }
        }

        Enemy_Creat[Turret_Number].players_that_can_be_hit = Enemy_Creat[Turret_Number].player_in_range[zero2];

    }
    void enemy_ai(void)
    {
        DrawModelEx(Enemy_Creat[Turret_Number].turret, Enemy_Creat[Turret_Number].enemy_pos, rotation, Enemy_Creat[Turret_Number].enemy_rotation - 90.0f, Turret_scale, BLUE);
        DrawModel(Turret_Bottom, (Vector3){Enemy_Creat[Turret_Number].enemy_pos.x, 0.0f, Enemy_Creat[Turret_Number].enemy_pos.z}, 1.0f, BLUE);
        DrawModelWiresEx(Enemy_Creat[Turret_Number].turret, Enemy_Creat[Turret_Number].enemy_pos, rotation, Enemy_Creat[Turret_Number].enemy_rotation - 90.0f, Turret_scale, DARKBLUE);
        Vector3 circle_rotation = {1,0,0};
        DrawCircle3D(Enemy_Creat[Turret_Number].enemy_pos, 50.0f, circle_rotation, 90.0f, RED);
        if(Enemy_Creat[Turret_Number].Turret_summon == true)
        {
            turret_algorithm();

            char_position_x = ((players[Enemy_Creat[Turret_Number].players_that_can_be_hit].Player_hit_box.min.x + (players[Enemy_Creat[Turret_Number].players_that_can_be_hit].Player_hit_box_size.x/2)) - Enemy_Creat[Turret_Number].enemy_pos.x)/hit_radius;
            char_position_z = ((players[Enemy_Creat[Turret_Number].players_that_can_be_hit].Player_hit_box.min.z + (players[Enemy_Creat[Turret_Number].players_that_can_be_hit].Player_hit_box_size.z/2)) - Enemy_Creat[Turret_Number].enemy_pos.z)/hit_radius;
            rotation_cos = atan2(char_position_z, char_position_x);
            rotation_cos = (rotation_cos * 180.0f) / M_PI + 180.0f;
            if(round(rotation_cos) - (360 - Enemy_Creat[Turret_Number].enemy_rotation) > 0.0f && round(rotation_cos) - (360 - Enemy_Creat[Turret_Number].enemy_rotation) < 180.0f
            || round(rotation_cos) - (360 - Enemy_Creat[Turret_Number].enemy_rotation) < -180.0f)
            {
                Enemy_Creat[Turret_Number].enemy_rotation -= 1.0f;
            }
            if(round(rotation_cos) - (360 - Enemy_Creat[Turret_Number].enemy_rotation) < 0.0f && round(rotation_cos) - (360 - Enemy_Creat[Turret_Number].enemy_rotation) > -180.0f
            || round(rotation_cos) - (360 - Enemy_Creat[Turret_Number].enemy_rotation) > 180.0f)
            {
                Enemy_Creat[Turret_Number].enemy_rotation += 1.0f;
            }
            if(Enemy_Creat[Turret_Number].enemy_rotation >= 360.0f)
            {
                Enemy_Creat[Turret_Number].enemy_rotation -= 360.0f;
            }
            if(Enemy_Creat[Turret_Number].enemy_rotation <= 0.0f)
            {
                Enemy_Creat[Turret_Number].enemy_rotation += 360.0f;
            }

            Enemy_Creat[Turret_Number].length = sqrt(pow(players[Enemy_Creat[Turret_Number].players_that_can_be_hit].Player.x - Enemy_Creat[Turret_Number].enemy_pos.x, 2) + pow(players[Enemy_Creat[Turret_Number].players_that_can_be_hit].Player.z - Enemy_Creat[Turret_Number].enemy_pos.z, 2));
            Enemy_Creat[Turret_Number].length = atan2(Enemy_Creat[Turret_Number].length, (players[Enemy_Creat[Turret_Number].players_that_can_be_hit].Player.y - Enemy_Creat[Turret_Number].enemy_pos.y)) + -0.5 * M_PI;
            Enemy_Creat[Turret_Number].turret.transform = MatrixRotateX(Enemy_Creat[Turret_Number].length);
            enemy_ai_bullet();
        }
    }


    Jump_Factor = 0.15 * 60 / GetFPS();

    SetExitKey(KEY_ESCAPE);

    int Bullet_timer = 0;

    void player_hitscan(void)
    {
        for(Turret_Number = 0; Turret_Number > Spawnable_Turrets; Turret_Number++)
        {
            RayCollision test_1 = GetRayCollisionSphere(GetScreenToWorldRay((Vector2){half_screen.x, half_screen.y - 100}, Camera_Unbound), Enemy_Creat[Turret_Number].enemy_pos, 0.5f);
            if(IsMouseButtonDown(MOUSE_BUTTON_LEFT) && test_1.hit == true)
            {
                Enemy_Creat[Turret_Number].Health_points -= 100;
            }
        }
    }

    void player_bullet(void)
    {
        if(Bullet_timer != 1 && IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        {

            Bullet_Number ++;
            bullet_string[Bullet_Number].death = false;
            bullet_string[Bullet_Number].Bullet_Position = (Vector3){players[your_player].Player.x - MouseZChangeSin / 7, Camera_Unbound.target.y,  players[your_player].Player.z + MouseXChangeCos / 7};
            bullet_string[Bullet_Number].Bullet_Start = players[your_player].Player;

            float test2 = GetMouseY();
            float test = GetScreenHeight();

            bullet_string[Bullet_Number].Bullet_Speed = (Vector3)
            {
                (-MouseXChangeCos)/GetFPS() * 10,
              ((Camera_Unbound.target.y - Camera_Unbound.position.y))/GetFPS() * 10,
                (-MouseZChangeSin)/GetFPS() * 10
            };

            Bullet_timer = 1;
        }

        for(int bullet_Select = 1 ;bullet_Select <= Bullet_Number; bullet_Select++)
        {

            if(bullet_string[bullet_Select].death == false)
            {
                bullet_string[bullet_Select].Bullet_Position = (Vector3)
                {
                    bullet_string[bullet_Select].Bullet_Position.x + bullet_string[bullet_Select].Bullet_Speed.x,
                    bullet_string[bullet_Select].Bullet_Position.y + bullet_string[bullet_Select].Bullet_Speed.y,
                    bullet_string[bullet_Select].Bullet_Position.z + bullet_string[bullet_Select].Bullet_Speed.z
                };
                DrawSphere(bullet_string[bullet_Select].Bullet_Position, 0.50f, GREEN);
            }
            for(Turret_Number = 0; Turret_Number < 3; Turret_Number++)
            {
                if(
                    bullet_string[bullet_Select].death == false
                &&
                    CheckCollisionSpheres(bullet_string[bullet_Select].Bullet_Position, 0.50f, Enemy_Creat[Turret_Number].enemy_pos, 1.0f))
                {
                    Enemy_Creat[Turret_Number].Health_points -= 2000;
                    bullet_string[bullet_Select].death = true;
                    Bullet_Death ++;
                }
            for(zero = 0; zero <= Active_Players; zero ++)
            {
                if(CheckCollisionBoxSphere(players[zero].Player_hit_box, bullet_string[bullet_Select].Bullet_Position, 1.0f))
                {
                    players[zero].Health_points -= 10;
                    bullet_string[bullet_Select].death = true;
                }
            }

            }
            for(zero = 0; zero <= Active_Players; zero ++)
            {
            }
            if(sqrt(pow(bullet_string[Bullet_Number].Bullet_Start.x - bullet_string[bullet_Select].Bullet_Position.x, 2)
            + pow(bullet_string[Bullet_Number].Bullet_Start.z - bullet_string[bullet_Select].Bullet_Position.z, 2) ) >= players[your_player].Bullet_Range)
            {
                bullet_string[bullet_Select].death = true;
            }

        }
        if(Bullet_timer != 0)
        {
            fps_bullets ++;
        }
        if(fps_bullets == target_fps)
        {
            fps_bullets = 0;
            Bullet_timer = 0;
        }
    }

    void player_variables_1(void)
    {
        players[your_player].player_color_counter = 1;
        players[your_player].Max_Health = 1000;
        players[your_player].Health_points = 1000;
        players[your_player].speed_variable = 2.5f;
        players[your_player].Max_Dash = 1;
        players[your_player].Dash = 1;
        players[your_player].Jump_Factor = 1.25f;
        players[your_player].Player_Character = BoonBoomChanger;
        players[your_player].Player_hit_box_size = (Vector3){0.5f, 5.0f, 0.5f};
    }

    void other_player_variables_1(void)
    {
        Active_Players += 1;
        players[Active_Players].Player = (Vector3)
        {(players[your_player].Player.x - MouseZChangeSin / 7) - players[your_player].Player_hit_box_size.x/2,
          players[your_player].Player.y,
         (players[your_player].Player.z + MouseXChangeCos / 7) - players[your_player].Player_hit_box_size.z/2};
        players[Active_Players].player_color_counter = 1;
        players[Active_Players].Max_Health = 1000;
        players[Active_Players].Health_points = 600;
        players[Active_Players].Player_Character = BoonBoomChanger;
        players[Active_Players].Player_hit_box_size = (Vector3){0.5f, 5.0f, 0.5f};
    }
    void other_player_color_1(void)
    {
        players[zero].Player_hit_box = (BoundingBox)
        {
            (Vector3)
            {
                players[zero].Player.x - players[zero].Player_hit_box_size.x / 2.0f,
                players[zero].Player.y,
                players[zero].Player.z - players[zero].Player_hit_box_size.z / 2.0f
            },
            (Vector3)
            {
                players[zero].Player.x + players[zero].Player_hit_box_size.x / 2.0f,
                players[zero].Player.y + players[zero].Player_hit_box_size.y / 2.0f,
                players[zero].Player.z + players[zero].Player_hit_box_size.z / 2.0f
            }
        };
        DrawModel(players[zero].Player_Character, players[zero].Player, 0.01f, BLUE);
        DrawBoundingBox(players[zero].Player_hit_box, BLUE);
    }
    void player_variables_2(void)
    {
        players[your_player].player_color_counter = 2;
        players[your_player].Max_Health = 250;
        players[your_player].Health_points = 250;
        players[your_player].speed_variable = 3.5f;
        players[your_player].Max_Dash = 2;
        players[your_player].Dash = 2;
        players[your_player].Bullet_Range = 250.0f;
        players[your_player].Jump_Factor = 2.0f;
        players[your_player].Player_hit_box_size = (Vector3){0.5f, 1.0f, 0.5f};

    }
    void player_variables_3(void)
    {
        players[your_player].player_color_counter = 3;
        players[your_player].Max_Health = 50;
        players[your_player].Health_points = 50;
        players[your_player].speed_variable = 1.5f;
        players[your_player].Max_Dash = 3;
        players[your_player].Dash = 3;
        players[your_player].Bullet_Range = 150.0f;
        players[your_player].Jump_Factor = 2.0f;
        players[your_player].Player_hit_box_size = (Vector3){0.6f, 1.1f, 0.6f};

    }
    void player_color_1(void)
    {


        if(players[your_player].Health_points > 0)
        {
            DrawModelEx(players[your_player].Player_Character, (Vector3){players[your_player].Player.x - MouseZChangeSin / 7, players[your_player].Player.y, players[your_player].Player.z + MouseXChangeCos / 7}, rotation, MouseXPercent * (-86 / 1.5) , scale, WHITE);
            players[your_player].Player_hit_box =
            (BoundingBox)
            {(Vector3){(players[your_player].Player.x - MouseZChangeSin / 7) - players[your_player].Player_hit_box_size.x/2, players[your_player].Player.y, (players[your_player].Player.z + MouseXChangeCos / 7) - players[your_player].Player_hit_box_size.z/2}
            , (Vector3){(players[your_player].Player.x - MouseZChangeSin / 7) + players[your_player].Player_hit_box_size.x/2, players[your_player].Player.y + players[your_player].Player_hit_box_size.y/2, (players[your_player].Player.z + MouseXChangeCos / 7) + players[your_player].Player_hit_box_size.z/2}};
            DrawBoundingBox(players[your_player].Player_hit_box, BLUE);
            player_hitscan();
        }
    }
    void player_color_2(void)
    {
        if(players[your_player].Health_points > 0)
        {
            players[your_player].Player_hit_box =
            (BoundingBox)
            {(Vector3){players[your_player].Player.x - players[your_player].Player_hit_box_size.x/2, players[your_player].Player.y, players[your_player].Player.z - players[your_player].Player_hit_box_size.z/2},
             (Vector3){players[your_player].Player.x + players[your_player].Player_hit_box_size.x/2, players[your_player].Player.y + players[your_player].Player_hit_box_size.y/2, players[your_player].Player.z + players[your_player].Player_hit_box_size.z/2}};

            DrawBoundingBox(players[your_player].Player_hit_box, RED);

            DrawCubeV((Vector3){players[your_player].Player.x - MouseZChangeSin / 7, players[your_player].Player.y + 0.25f, players[your_player].Player.z + MouseXChangeCos / 7}, Box_3d, RED);
            DrawCube((Vector3){players[your_player].Player.x - MouseZChangeSin / 7, players[your_player].Player.y + 0.75f, players[your_player].Player.z + MouseXChangeCos / 7}, 0.5f, 0.5f, 0.5f, ORANGE);
            player_bullet();
        }

    }
    void player_color_3(void)
    {
        players[your_player].Player_hit_box = (BoundingBox)
        {(Vector3){players[your_player].Player.x - players[your_player].Player_hit_box_size.x/2, players[your_player].Player.y, players[your_player].Player.z - players[your_player].Player_hit_box_size.z/2},
         (Vector3){players[your_player].Player.x + players[your_player].Player_hit_box_size.x/2, players[your_player].Player.y + players[your_player].Player_hit_box_size.y, players[your_player].Player.z + players[your_player].Player_hit_box_size.z/2}};
        DrawBoundingBox(players[your_player].Player_hit_box, PURPLE);
        DrawCubeV((Vector3){players[your_player].Player.x, players[your_player].Player.y + 0.25f, players[your_player].Player.z}, Box_3d, GREEN);
        DrawCube((Vector3){players[your_player].Player.x, players[your_player].Player.y + 0.75f, players[your_player].Player.z }, 0.5f, 0.5f, 0.5f, PURPLE);
        player_bullet();
    }

    ToggleFullscreen();

    SetTargetFPS(target_fps);

    HideCursor();
    bool cursor_is_hidden = true;

    Rectangle button_1 = {screen_size.x - (1575 * scale_factor), screen_size.y / (5 * scale_factor), 1050, 150};
    Rectangle button_2 = {screen_size.x - (1575 * scale_factor), GetScreenHeight() / ((5 + 150) * scale_factor), 525, 150};

    SetMousePosition(screen_size.x/2, screen_size.y * 0.75);

    player_variables_1();

    while(!WindowShouldClose())
    {

        MousePos.x = GetMouseX();
        MousePos.y = GetMouseY();

        if(MousePos.x >= screen_size.x - 1)
        {
            mouseY = GetMouseY();           //sets X axis to loop
            SetMousePosition(6, mouseY);
        }
        if(MousePos.x <=  1)
        {
            mouseY = GetMouseY();
            SetMousePosition(screen_size.x + 1, mouseY);
        }

        if(title_screen == true)
        {
            if(cursor_is_hidden == false)
            {
                HideCursor();
                cursor_is_hidden = true;
            }
            BeginDrawing();

            ClearBackground(BLUE);
            Rectangle Mouse_Box = {MousePos.x - 10.0f, MousePos.y - 10.0f, 20, 20};

            // IP Address entering
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                collision = 0;
                if(CheckCollisionRecs(button_1, Mouse_Box))
                {
                    collision = 1;
                }
                if(CheckCollisionRecs(button_2, Mouse_Box))
                {
                    collision = 2;
                }
            }
            if(collision == 1 && IP_PLACE < 12)
            {
                DrawRectangleRec(button_1, GRAY);

                int ch = GetKeyPressed();

                if(ch != NULL && ch < 58 && ch > 47 )
                {
                    IP_ADDRESS[IP_PLACE] = ch - 48;
                    IP_PLACE ++;
                    fps_seconds = target_fps / 2;
                }
                if(IsKeyPressed(KEY_BACKSPACE))
                {
                    IP_ADDRESS[IP_PLACE] = 0;
                    IP_PLACE --;
                    IP_ADDRESS[IP_PLACE] = 0;
                    fps_seconds = target_fps / 2;
                }
                if(IP_PLACE < 0)
                {
                    IP_PLACE = 0;
                }

            }else
            {
                DrawRectangleRec(button_1, RAYWHITE);
                if(IP_PLACE >= 12)
                {
                    collision = 0;
                    IP_PLACE = 11;
                }
            }

            // Port entering

            if(collision == 2)
            {
                DrawRectangleRec(button_2, GRAY);
                int ch = GetKeyPressed();

                if(ch != NULL && ch < 58 && ch > 47 )
                {
                    port_number = (port_number * 10) + (ch - 48);
                }
                if(IsKeyPressed(KEY_BACKSPACE))
                {
                    port_number = port_number/10;
                }

            }else
            {
                DrawRectangleRec(button_2, WHITE);
            }

            octet_1 = 0;
            octet_2 = 0;
            octet_3 = 0;
            octet_4 = 0;
            for(int i = 0; i < 3; i++)
            {
                octet_1 = octet_1 * 10 + IP_ADDRESS[i];
            }
            for(int i = 3; i < 6; i++)
            {
                octet_2 = octet_2 * 10 + IP_ADDRESS[i];
            }
            for(int i = 6; i < 9; i++)
            {
                octet_3 = octet_3 * 10 + IP_ADDRESS[i];
            }
            for(int i = 9; i < 12; i++)
            {
                octet_4 = octet_4 * 10 + IP_ADDRESS[i];
            }
            if(fps_seconds == target_fps)
            {
                fps_seconds = 0;
            }

            if(fps_seconds/(target_fps / 2) && IP_PLACE < 12)
            {
                DrawRectangle(GetScreenWidth() - GetScreenWidth() * 1588 / 1920 + (1050 * (IP_PLACE) / 12) + 25, screen_size.y / 5 + 45, 50, 15, BLACK);
            }

            DrawText(TextFormat("%i",IP_ADDRESS[0]),GetScreenWidth() - GetScreenWidth() * 1575 / 1920 + 25, screen_size.y / 5 , 50, BLACK);
            DrawText(TextFormat("%i",IP_ADDRESS[11]),GetScreenWidth() - GetScreenWidth() * 1575 / 1920 + (1050 * 11) / 12 + 25, screen_size.y / 5 , 50, BLACK);
            DrawText(TextFormat("%i",IP_ADDRESS[10]),GetScreenWidth() - GetScreenWidth() * 1575 / 1920 + (1050 * 10) / 12 + 25, screen_size.y / 5 , 50, BLACK);
            DrawText(TextFormat("%i",IP_ADDRESS[9]),GetScreenWidth() - GetScreenWidth() * 1575 / 1920 + (1050 * 9) / 12 + 25, screen_size.y / 5 , 50, BLACK);
            DrawText(TextFormat("%i",IP_ADDRESS[8]),GetScreenWidth() - GetScreenWidth() * 1575 / 1920 + (1050 * 8) / 12 + 25, screen_size.y / 5 , 50, BLACK);
            DrawText(TextFormat("%i",IP_ADDRESS[7]),GetScreenWidth() - GetScreenWidth() * 1575 / 1920 + (1050 * 7) / 12 + 25, screen_size.y / 5 , 50, BLACK);
            DrawText(TextFormat("%i",IP_ADDRESS[6]),GetScreenWidth() - GetScreenWidth() * 1575 / 1920 + (1050 * 6) / 12 + 25, screen_size.y / 5 , 50, BLACK);
            DrawText(TextFormat("%i",IP_ADDRESS[5]),GetScreenWidth() - GetScreenWidth() * 1575 / 1920 + (1050 * 5) / 12 + 25, screen_size.y / 5 , 50, BLACK);
            DrawText(TextFormat("%i",IP_ADDRESS[4]),GetScreenWidth() - GetScreenWidth() * 1575 / 1920 + (1050 * 4) / 12 + 25, screen_size.y / 5 , 50, BLACK);
            DrawText(TextFormat("%i",IP_ADDRESS[3]),GetScreenWidth() - GetScreenWidth() * 1575 / 1920 + (1050 * 3) / 12 + 25, screen_size.y / 5 , 50, BLACK);
            DrawText(TextFormat("%i",IP_ADDRESS[2]),GetScreenWidth() - GetScreenWidth() * 1575 / 1920 + (1050 * 2) / 12 + 25, screen_size.y / 5 , 50, BLACK);
            DrawText(TextFormat("%i",IP_ADDRESS[1]),GetScreenWidth() - GetScreenWidth() * 1575 / 1920 + (1050 / 12) + 25 , GetScreenHeight() / 5 , 50, BLACK);

            DrawText(TextFormat("%i",port_number),GetScreenWidth() - GetScreenWidth() * 1575 / 1920 + 25, GetScreenHeight() / 5 + 150, 50, BLACK);


            DrawFPS(10,10);


            fps_seconds++;

            if(IsKeyPressed(KEY_ENTER))
            {
                title_screen = false;
                combat = true;
            }


            DrawRectangleRec(Mouse_Box, BLACK);

            EndDrawing();
        }
        if(character_select == true)
        {
            if(cursor_is_hidden == true)
            {
                ShowCursor();
                cursor_is_hidden = false;
            }
            BeginDrawing();

            EndDrawing();
        }
        if(combat == true)
        {
            if(cursor_is_hidden == false)
            {
                HideCursor();
                cursor_is_hidden = true;
            }

            if(IsKeyReleased(KEY_LEFT_SHIFT) && Dashing == false)
            {
                dash_timer_bool = true;
            }
            if(dash_timer < target_fps * 0.125f && IsKeyPressed(KEY_LEFT_SHIFT) && dash_timer_bool == true && players[your_player].Dash >= 0.5f)
            {
                players[your_player].Dash -= 0.5f;
                Dashing = true;
                dash_timer_bool = false;
            }
            if(dash_timer_bool == true && dash_timer <= target_fps * 0.125f)
            {
                dash_timer += 1;
            }
            else
            {
                dash_timer = 0;
                dash_timer_bool = false;
            }
            if(Dashing == true && Dashing_Timer < target_fps)
            {
                Dashing_Timer += 1;
                if(IsKeyDown(KEY_W) && IsKeyDown(KEY_A)
                || IsKeyDown(KEY_W) && IsKeyDown(KEY_D)
                || IsKeyDown(KEY_S) && IsKeyDown(KEY_A)
                || IsKeyDown(KEY_S) && IsKeyDown(KEY_D))
                {
                    speed = players[your_player].speed_variable * (2.5f / GetFPS()) / 2;
                }
                else
                {
                    speed = players[your_player].speed_variable * (2.5f / GetFPS());
                }

            }
            if(Dashing_Timer == target_fps)
            {
                Dashing = false;
                Dashing_Timer = 0;
            }
            if(players[your_player].Dash < players[your_player].Max_Dash && Dashing == false)
            {
                players[your_player].Dash += 0.05f/target_fps;
            }

            if(IsKeyDown(KEY_W) && IsKeyDown(KEY_A)
            || IsKeyDown(KEY_W) && IsKeyDown(KEY_D)
            || IsKeyDown(KEY_S) && IsKeyDown(KEY_A)
            || IsKeyDown(KEY_S) && IsKeyDown(KEY_D))
            {
                if(IsKeyDown(KEY_LEFT_SHIFT) && players[your_player].Dash > 0.0f && Dashing == false)
                {
                    speed = (players[your_player].speed_variable * 1.5 / 2) / GetFPS();
                    players[your_player].Dash -= 0.5f / GetFPS();

                }
                else if(Dashing == false)
                {
                    speed = (players[your_player].speed_variable / 2) / GetFPS();
                }
            }
            else
            {
                if(IsKeyDown(KEY_LEFT_SHIFT) && players[your_player].Dash > 0.0f && Dashing == false)
                {
                    speed = (players[your_player].speed_variable * 1.5) / GetFPS();
                    players[your_player].Dash -= 0.5f / GetFPS();

                }
                else if(Dashing == false)
                {
                    speed = players[your_player].speed_variable / GetFPS();
                }

            }

            if(IsKeyDown(KEY_S))
            {
                Future_Position_x = camera.position.x;
                Current_Position_x = players[your_player].Player.x;
                Position_Squarred_x = Future_Position_x - Current_Position_x;
                Position_Squarred_x = Position_Squarred_x * speed;
                players[your_player].Player.x += Position_Squarred_x;

                Future_Position_z = camera.position.z;
                Current_Position_z = players[your_player].Player.z;
                Position_Squarred_z = Future_Position_z - Current_Position_z;
                Position_Squarred_z = Position_Squarred_z * speed;
                players[your_player].Player.z += Position_Squarred_z;
            }
            if(IsKeyDown(KEY_W))
            {
                Future_Position_x = camera.position.x;
                Current_Position_x = players[your_player].Player.x;
                Position_Squarred_x = Future_Position_x - Current_Position_x;
                Position_Squarred_x = Position_Squarred_x * speed;
                players[your_player].Player.x -= Position_Squarred_x;

                Future_Position_z = camera.position.z;
                Current_Position_z = players[your_player].Player.z;
                Position_Squarred_z = Future_Position_z - Current_Position_z;
                Position_Squarred_z = Position_Squarred_z * speed;
                players[your_player].Player.z -= Position_Squarred_z;
            }
            if(IsKeyDown(KEY_A))
            {
                Future_Position_x = camera.position.z;
                Current_Position_x = players[your_player].Player.z;
                Position_Squarred_x = Future_Position_x - Current_Position_x;
                Position_Squarred_x = Position_Squarred_x * speed;
                players[your_player].Player.x -= Position_Squarred_x;

                Future_Position_z = -camera.position.x;
                Current_Position_z = -players[your_player].Player.x;
                Position_Squarred_z = Future_Position_z - Current_Position_z;
                Position_Squarred_z = Position_Squarred_z * speed;
                players[your_player].Player.z -= Position_Squarred_z;
            }
            if(IsKeyDown(KEY_D))
            {
                Future_Position_x = camera.position.z;
                Current_Position_x = players[your_player].Player.z;
                Position_Squarred_x = Future_Position_x - Current_Position_x;
                Position_Squarred_x = Position_Squarred_x * speed;
                players[your_player].Player.x += Position_Squarred_x;

                Future_Position_z = -camera.position.x;
                Current_Position_z = -players[your_player].Player.x;
                Position_Squarred_z = Future_Position_z - Current_Position_z;
                Position_Squarred_z = Position_Squarred_z * speed;
                players[your_player].Player.z += Position_Squarred_z;
            }
            if(IsKeyPressed(KEY_SPACE))
            {
                jump = true;
            }
            if(jump == true)
            {
                fps_seconds += 1;
                float holding = target_fps;
                Square_Jump_hold = ((2.0f * (fps_seconds / holding)) - sqrt(players[your_player].Jump_Factor));
                Square_Jump_hold = pow(Square_Jump_hold, 2);
                Square_Jump_hold = -Square_Jump_hold + players[your_player].Jump_Factor;
                players[your_player].Player.y = Square_Jump_hold;
            }
            if(players[your_player].Player.y <= 0.0f && fps_seconds >= 1)
            {
                jump = false;
                fps_seconds = 0;
                players[your_player].Player.y = 0.0f;
            }
            if(IsKeyPressed(KEY_T))
            {
                Turret_Select();
            }

            MouseXPercent = MousePos.x / screen_size.x;
            MouseXPercent = MouseXPercent * M_PI;
            MouseXPercent = MouseXPercent * 2;

            MouseZChangeSin = sin(MouseXPercent);
            MouseXChangeCos = cos(MouseXPercent);

            MouseZChangeSin = MouseZChangeSin * camera_distance;
            MouseXChangeCos = MouseXChangeCos * camera_distance;

            camera.position.z = MouseZChangeSin + players[your_player].Player.z;
            camera.position.x = MouseXChangeCos + players[your_player].Player.x;
            //this entire thing is to set the camera to the player and get the mouse correlated with the camera and the players movement is mased off of this camera so it must be a constant

            MouseYPercent = MousePos.y / screen_size.y;
            Camera_Unbound.position.x =  camera.position.x;
            Camera_Unbound.position.z =  camera.position.z;

            Camera_Unbound.target.y = 5 * MouseYPercent + players[your_player].Player.y - 0.1f;
            Camera_Unbound.position.y =  4.9f + players[your_player].Player.y - Camera_Unbound.target.y + players[your_player].Player.y;

            Camera_Unbound.target.x = players[your_player].Player.x;
            Camera_Unbound.target.z = players[your_player].Player.z;

            BeginDrawing();

            BeginMode3D(Camera_Unbound);

            ClearBackground(RAYWHITE);

            camera.target = players[your_player].Player; // third person camera is looking at player/ firsrt person camera

            DrawGrid(1000, 1.0f);
            DrawLine3D(Z_Axis_Start, Z_Axis_End, RED);
            DrawLine3D(X_Axis_Start, X_Axis_End, BLUE);

            if(IsKeyPressed(KEY_E))
            {
                player_color_counter += 1;
                if(player_color_counter == 4)
                {
                    player_color_counter = 1;
                }
                switch (player_color_counter)
                {
                    case 1:
                    player_variables_1();
                        break;
                    case 2:
                    player_variables_2();
                        break;
                    case 3:
                    player_variables_3();
                        break;
                }
            }
            if(IsKeyPressed(KEY_Q))
            {
                player_color_counter -= 1;
                if(player_color_counter == 0)
                {
                    player_color_counter = 3;
                }
                switch (player_color_counter)
                {
                    case 1:
                    player_variables_1();
                        break;
                    case 2:
                    player_variables_2();
                        break;
                    case 3:
                    player_variables_3();
                        break;
                }

            }
            if(IsKeyPressed(KEY_R))
            {
                other_player_variables_1();
            }

            for(Turret_Number = 0; Turret_Number < Spawnable_Turrets ; Turret_Number++)
            {
                if(Enemy_Creat[Turret_Number].Turret_summon == true)
                {
                    enemy_ai();
                }
            }
            for(zero = 1; zero <= Active_Players; zero++)
            {
                if(players[zero].Health_points >= 0)
                {
                    switch(players[zero].player_color_counter)
                    {
                        case 1:
                            other_player_color_1();
                            break;
                    }
                }
                else
                {
                    players[zero].Player = (Vector3){-9999.9f, -9999.9f, -9999.9f};
                    players[zero].Player_hit_box = (BoundingBox){players[zero].Player,
                    (Vector3){players[zero].Player.x + players[zero].Player_hit_box_size.x, players[zero].Player.y + players[zero].Player_hit_box_size.y, players[zero].Player.z + players[zero].Player_hit_box_size.z}};
                }
            }
            switch (players[your_player].player_color_counter)
            {
                case 1:
                    player_color_1();
                    break;
                case 2:
                    player_color_2();
                    break;
                case 3:
                    player_color_3();
                    break;
            }

            DrawCubeWires(players[your_player].Player, 0.8f, 0.6f, 0.8f, PURPLE);
            DrawRay(GetScreenToWorldRay(half_screen, Camera_Unbound), PINK);
            UpdateCamera(&camera, CAMERA_CUSTOM);
            UpdateCamera(&Camera_Unbound, CAMERA_CUSTOM);
            EndMode3D();

            DrawCircle(half_screen.x, half_screen.y - 100, 5, PINK);
            hp_percentage = 1000 / players[your_player].Max_Health;
            Rectangle Health_Bar = {screen_size.x / 2 - 500, screen_size.y * 0.8,players[your_player].Max_Health * hp_percentage, 75};
            Rectangle True_Health = {screen_size.x / 2 - 500, screen_size.y * 0.8, players[your_player].Health_points * hp_percentage, 75};
            DrawRectangleRec(Health_Bar, GOLD);
            DrawRectangleRec(True_Health, BLUE);

            DrawText(TextFormat("%i", Enemy_Creat[0].players_that_can_be_hit), 300, 300, 20, BLUE);
            DrawText(TextFormat("%i", Active_Players), 300, 350, 20, BLUE);
            DrawFPS(10,10);
            EndDrawing();

        }


    }
    UnloadModel(BoonBoomChanger);
    UnloadModel(Turret_Top);
    UnloadModel(Turret_Bottom);
    CloseWindow();

}
