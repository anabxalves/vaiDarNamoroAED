/*******************************************************************************************
*
*   raylib [core] examples - basic screen manager
*
*   NOTE: This example illustrates a very simple screen manager based on a states machines
*
*   Example originally created with raylib 4.0, last time updated with raylib 4.0
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2021-2023 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include <stdio.h>
#include "raylib.h"

//------------------------------------------------------------------------------------------
// Types and Structures Definition
//------------------------------------------------------------------------------------------
typedef enum GameScreen { LOGO = 0, TITLE, GAMEPLAY, ENDING } GameScreen;

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
    GameScreen currentScreen = LOGO;

    // TODO: Initialize all required variables and load all required data here!
    Sound audioVaiDarNamoro;
    Sound audioIra;
    Sound audioEleGosta;
    InitAudioDevice();
    
    // audio jogo
    audioVaiDarNamoro = LoadSound("/Users/anabxalves/Desktop/CESAR/AED/vdnAed/vdnAed/resources/audios/vai-dar-namoro---rodrigo-faro-made-with-Voicemod-technology.mp3");
    SetSoundVolume(audioVaiDarNamoro, 1.5);
    audioIra = LoadSound("/Users/anabxalves/Desktop/CESAR/AED/vdnAed/vdnAed/resources/audios/ira-rodrigo-faro-made-with-Voicemod-technology.mp3");
    SetSoundVolume(audioIra, 1.5);
    audioEleGosta = LoadSound("/Users/anabxalves/Desktop/CESAR/AED/vdnAed/vdnAed/resources/audios/ele-gosta-vai-dar-namoro-2022.mp3");
    SetSoundVolume(audioEleGosta, 1.5);
    PlaySound(audioVaiDarNamoro);
    
    // logo screen
    Texture2D backgroundLogo = LoadTexture("/Users/anabxalves/Desktop/CESAR/AED/vdnAed/vdnAed/resources/pixil-frame-0-3.png");
    float scrollingBackLogo = 0.0f;
    
    // title screen
    const char msg1[50] = "CHEGOU A HORA DE DAR MACTH...";
    const char msg2[50] = "... com um dos nossos DISPUTADOS candidatos...";
    const char msg3[50] = "... aperte ENTER para jogar <3";
    
    Font font1 = LoadFont("/Users/anabxalves/Desktop/CESAR/AED/vdnAed/vdnAed/resources/custom_mecha.png");
    Font font2 = LoadFont("/Users/anabxalves/Desktop/CESAR/AED/vdnAed/vdnAed/resources/custom_alagard.png");
    Font font3 = LoadFont("/Users/anabxalves/Desktop/CESAR/AED/vdnAed/vdnAed/resources/custom_jupiter_crash.png");
    
    Texture2D backgroundTitle = LoadTexture("/Users/anabxalves/Desktop/CESAR/AED/vdnAed/vdnAed/resources/fundoVDNdesfocado.png");
    
    Vector2 fontPosition1 = { screenWidth/2.0f - MeasureTextEx(font1, msg1, (float)font1.baseSize, -3).x/2, screenHeight/2.0f - font1.baseSize/2.0f - 80.0f };
    Vector2 fontPosition2 = { screenWidth/2.0f - MeasureTextEx(font2, msg2, (float)font2.baseSize, -2.0f).x/2.0f, screenHeight/2.0f - font2.baseSize/2.0f - 10.0f };
    Vector2 fontPosition3 = { screenWidth/2.0f - MeasureTextEx(font3, msg3, (float)font3.baseSize, 2.0f).x/2.0f, screenHeight/2.0f - font3.baseSize/2.0f + 50.0f };
    
    float scrollingBackTitle = 0.0f;
    
    int framesCounter = 0;          // Useful to count frames

    SetTargetFPS(60);               // Set desired framerate (frames-per-second)
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        switch(currentScreen)
        {
            case LOGO:
            {
                // TODO: Update LOGO screen variables here!
                
                scrollingBackLogo -= 0.1f;
                if (scrollingBackLogo <= -backgroundLogo.width) scrollingBackLogo = 0;
                framesCounter++;    // Count frames

                // Wait for 10 seconds (600 frames) before jumping to TITLE screen
                if ((IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)) || framesCounter > 600)
                {
                    currentScreen = TITLE;
                }
            } break;
            case TITLE:
            {
                // TODO: Update TITLE screen variables here!
                
                scrollingBackTitle -= 0.1f;
                if (scrollingBackTitle <= -backgroundTitle.width) scrollingBackTitle = 0;

                // Press enter to change to GAMEPLAY screen
                if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
                {
                    currentScreen = GAMEPLAY;
                }
            } break;
            case GAMEPLAY:
            {
                // TODO: Update GAMEPLAY screen variables here!

                // Press enter to change to ENDING screen
                if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
                {
                    currentScreen = ENDING;
                }
            } break;
            case ENDING:
            {
                // TODO: Update ENDING screen variables here!

                // Press enter to return to TITLE screen
                if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
                {
                    currentScreen = TITLE;
                }
            } break;
            default: break;
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            switch(currentScreen)
            {
                case LOGO:
                {
                    // TODO: Draw LOGO screen here!
                    DrawTextureEx(backgroundLogo, (Vector2){ scrollingBackLogo, 0 }, 0.0f, 1.0f, WHITE);
                    DrawTextureEx(backgroundLogo, (Vector2){ (backgroundLogo.width) + scrollingBackLogo, 0 }, 0.0f, 1.0f, WHITE);
                    DrawText("VAI DAR NAMORO - CESAR´S VERSION", 10, 10, 20, RED);
                    DrawText("(c) Cana Productions", screenWidth - 150, screenHeight - 20, 10, RAYWHITE);
                    PlaySound(audioIra);
                } break;
                case TITLE:
                {
                    // TODO: Draw TITLE screen here!
                    DrawTextureEx(backgroundTitle, (Vector2){ scrollingBackTitle, 0 }, 0.0f, 1.0f, WHITE);
                    DrawTextureEx(backgroundTitle, (Vector2){ (backgroundTitle.width) + scrollingBackTitle, 0 }, 0.0f, 1.0f, WHITE);

                    DrawTextEx(font1, msg1, fontPosition1, (float)font1.baseSize, -3, WHITE);   // Font loading
                    DrawTextEx(font2, msg2, fontPosition2, (float)font2.baseSize, -2, WHITE);   // Font loading
                    DrawTextEx(font3, msg3, fontPosition3, (float)font3.baseSize, 2, WHITE);    // Font loading
                    PlaySound(audioEleGosta);
                } break;
                case GAMEPLAY:
                {
                    // TODO: Draw GAMEPLAY screen here!
                    DrawRectangle(0, 0, screenWidth, screenHeight, PURPLE);
                    DrawText("GAMEPLAY SCREEN", 20, 20, 40, MAROON);
                    DrawText("PRESS ENTER or TAP to JUMP to ENDING SCREEN", 130, 220, 20, MAROON);
                    PlaySound(audioIra);
                } break;
                case ENDING:
                {
                    // TODO: Draw ENDING screen here!
                    DrawRectangle(0, 0, screenWidth, screenHeight, BLUE);
                    DrawText("ENDING SCREEN", 20, 20, 40, DARKBLUE);
                    DrawText("PRESS ENTER or TAP to RETURN to TITLE SCREEN", 120, 220, 20, DARKBLUE);
                    PlaySound(audioVaiDarNamoro);
                } break;
                default: break;
            }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------

    // TODO: Unload all loaded data (textures, fonts, audio) here!
    
    // unload audio
    UnloadSound(audioVaiDarNamoro);
    
    // unload LOGO
    UnloadTexture(backgroundLogo);  // Unload background texture
    
    // unload TITLE
    UnloadFont(font1);      // Font unloading
    UnloadFont(font2);      // Font unloading
    UnloadFont(font3);      // Font unloading
    UnloadTexture(backgroundTitle);  // Unload background texture
    
    CloseAudioDevice();
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
