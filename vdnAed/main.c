/*******************************************************************************************
*
*   raylib [textures] example - Background scrolling
*
*   Example originally created with raylib 2.0, last time updated with raylib 2.5
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2019-2023 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include <stdio.h>

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "Vai Dar Namoro - Cesar´s Version");

    // NOTE: Be careful, background width must be equal or bigger than screen width
    // if not, texture should be draw more than two times for scrolling effect
    Texture2D background = LoadTexture("/Users/anabxalves/Desktop/CESAR/AED/vdnAed/vdnAed/resources/pixil-frame-0-3.png");
    // Texture2D midground = LoadTexture("/Users/anabxalves/Desktop/CESAR/AED/vdnAed/vdnAed/resources/b55013272fef7d6.png");
    // Texture2D foreground = LoadTexture("/Users/anabxalves/Desktop/CESAR/AED/vdnAed/vdnAed/resources/652876dd6f665bb.png");
    
    const char msg1[50] = "CHEGOU A HORA DE DAR MACTH...";
    const char msg2[50] = "... com um dos nossos DISPUTADOS candidatos...";
    const char msg3[50] = "... jogue AGORA <3";
    
    Font font1 = LoadFont("/Users/anabxalves/Desktop/CESAR/AED/vdnAed/vdnAed/resources/custom_mecha.png");
    Font font2 = LoadFont("/Users/anabxalves/Desktop/CESAR/AED/vdnAed/vdnAed/resources/custom_alagard.png");
    Font font3 = LoadFont("/Users/anabxalves/Desktop/CESAR/AED/vdnAed/vdnAed/resources/custom_jupiter_crash.png");

    float scrollingBack = 0.0f;
    // float scrollingMid = 0.0f;
    // float scrollingFore = 0.0f;
    
    Vector2 fontPosition1 = { screenWidth/2.0f - MeasureTextEx(font1, msg1, (float)font1.baseSize, -3).x/2,
                                  screenHeight/2.0f - font1.baseSize/2.0f - 80.0f };

    Vector2 fontPosition2 = { screenWidth/2.0f - MeasureTextEx(font2, msg2, (float)font2.baseSize, -2.0f).x/2.0f,
                                  screenHeight/2.0f - font2.baseSize/2.0f - 10.0f };

    Vector2 fontPosition3 = { screenWidth/2.0f - MeasureTextEx(font3, msg3, (float)font3.baseSize, 2.0f).x/2.0f,
                                  screenHeight/2.0f - font3.baseSize/2.0f + 50.0f };

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        scrollingBack -= 0.1f;
        // scrollingMid -= 0.5f;
        // scrollingFore -= 1.0f;

        // NOTE: Texture is scaled twice its size, so it sould be considered on scrolling
        if (scrollingBack <= -background.width) scrollingBack = 0;
        // if (scrollingMid <= -midground.width*2) scrollingMid = 0;
        // if (scrollingFore <= -foreground.width*2) scrollingFore = 0;
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(GetColor(0x052c46ff));

            // Draw background image twice
            // NOTE: Texture is scaled twice its size
            DrawTextureEx(background, (Vector2){ scrollingBack, 0 }, 0.0f, 1.0f, WHITE);
            DrawTextureEx(background, (Vector2){ (background.width) + scrollingBack, 0 }, 0.0f, 1.0f, WHITE);

            // Draw midground image twice
            // DrawTextureEx(midground, (Vector2){ scrollingMid, 20 }, 0.0f, 1.0f, WHITE);
            // DrawTextureEx(midground, (Vector2){ (midground.width) + scrollingMid, 20 }, 0.0f, 0.0f, WHITE);

            // Draw foreground image twice
            // DrawTextureEx(foreground, (Vector2){ scrollingFore, 70 }, 0.0f, 0.5f, WHITE);
            // DrawTextureEx(foreground, (Vector2){ (foreground.width) + scrollingFore, 70 }, 0.0f, 0.0f, WHITE);

            DrawText("VAI DAR NAMORO - CESAR´S VERSION", 10, 10, 20, RED);
            DrawText("(c) Cana Productions", screenWidth - 150, screenHeight - 20, 10, RAYWHITE);
        
            ClearBackground(RAYWHITE);

            DrawTextEx(font1, msg1, fontPosition1, (float)font1.baseSize, -3, WHITE);       // Font loading
            DrawTextEx(font2, msg2, fontPosition2, (float)font2.baseSize, -2, WHITE);       // Font loading
            DrawTextEx(font3, msg3, fontPosition3, (float)font3.baseSize, 2, WHITE);        // Font loading

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(background);  // Unload background texture
    // UnloadTexture(midground);   // Unload midground texture
    // UnloadTexture(foreground);  // Unload foreground texture
    
    UnloadFont(font1);      // Font unloading
    UnloadFont(font2);      // Font unloading
    UnloadFont(font3);      // Font unloading

    CloseWindow();              // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
