#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "raylib.h"
#define STORAGE_DATA_FILE   "/Users/anabxalves/Desktop/CESAR/AED/vdnAed/vdnAed/dB/storage.data"   // arq Highscore
#define MAX_INPUT_CHARS     9
#define NUM_FRAMES  3   // tamanho botão

typedef enum GameScreen { LOGO = 0, TITLE, START, HIGH_SCORE, JOGO, ENDING } GameScreen;

typedef struct respostaNode {
    char resposta[1000];
    struct respostaNode *proximo;
} RespostaNode;

typedef struct {
    char nome[50];
    int pontuacao;
} Jogador;

typedef enum {
    STORAGE_POSITION_SCORE = 0,
    STORAGE_POSITION_HISCORE = 1
} StorageData;

int mainJogo(int x); // é a main do jogo, falta integrar com o raylib
void atualizaRanking(Jogador ranking[], char *nome, int pontuacao, int *numJogadores);
void insertionSort(Jogador ranking[], int numJogadores);
void salvaRankingEmArquivo(Jogador ranking[], int numJogadores, const char *nomeArquivo);
void carregaRankingDeArquivo(Jogador ranking[], int *numJogadores, const char *nomeArquivo);
void lerPerguntasRespostas(char *perguntas[], char *respostas[], const char *perguntasJogo, const char *respostaJogo, int *tamanho);
RespostaNode* addRedposta(RespostaNode *head, char *resposta);
void liberaLista(RespostaNode *head);
void jogo(char *perguntas[], char *respostas[], int numPerguntas, int *pontuacao);
static bool SaveStorageValue(unsigned int position, int value);
static int LoadStorageValue(unsigned int position);

// Globais
Jogador ranking[100];
int numJogadores = 0;

int main(void)
{
    // Inicialização
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1280;
    const int screenHeight = 720;
    
    InitWindow(screenWidth, screenHeight, "Vai Dar Namoro - Cesar´s Version");
    GameScreen currentScreen = LOGO;
    
    // TODO: Inicialização das variáveis e carregamento de todos os dados necessários!
    // Variaveis Logica
        // main
        // char nome[50]; ta como name (linha 70)
        int pontuacao = 0;
        int numPerguntas = 0;
        char *perguntas[100];
        char *respostas[100];
        Jogador rankingAna[100];
        Jogador rankingCaio[100];
        int numJogadoresAna = 0;
        int numJogadoresCaio = 0;
    
        // highscore
        int score = 0;
        int hiscore = 0;
    
        // input nome
        char name[MAX_INPUT_CHARS + 1] = "\0";      // NOTE: One extra space required for null terminator char '\0' / fica armazenado dentro da execução
        int letterCountName = 0;
        Rectangle textBox = { screenWidth/2.0f - 100, 180, 225, 50 };
        bool mouseOnText = false;
    
        // botao next
        Texture2D button = LoadTexture("/Users/anabxalves/Desktop/CESAR/AED/vdnAed/vdnAed/resources/button.png"); // Load button texture
        float frameHeight = (float)button.height/NUM_FRAMES;
        Rectangle sourceRec = { 0, 0, (float)button.width, frameHeight };
        Rectangle btnBounds = { screenWidth/2.0f - button.width/2.0f, screenHeight/1.25f - button.height/NUM_FRAMES/2.0f, (float)button.width, frameHeight }; // Define button bounds on screen
        int btnState = 0;               // Button state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED
        bool btnAction = false;         // Button action should be activated
        Vector2 mousePoint = { 0.0f, 0.0f };
    
        // entrada seletor caio ou ana
        char selected[MAX_INPUT_CHARS + 1] = "\0";
        int letterCountSelector = 0;
        Rectangle selectorBox = { screenWidth/2.0f - 100, screenHeight/2.0f + 10, 225, 50 };
        bool mouseOnSelector = false;
    
    // Variaveis Front Jogo
        // audio jogo
        Sound audioVaiDarNamoro;
        Sound audioIra;
        Sound audioEleGosta;
        InitAudioDevice();

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
    
    int framesCounter = 0;          // contador de frames

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // detecta fechamento da janela ou ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        switch(currentScreen)
        {
            case LOGO:
            {
                // variaveis da tela LOGO
                
                scrollingBackLogo -= 0.1f;
                if (scrollingBackLogo <= -backgroundLogo.width) scrollingBackLogo = 0;
                framesCounter++;    // Count frames

                // espera 10 segundos (600 frames) antes de ir automaticamente para a tela TITLE
                if ((IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)) || framesCounter > 600) currentScreen = TITLE;
            } break;
            case TITLE:
            {
                // variaveis da tela TITLE
                
                scrollingBackTitle -= 0.1f;
                if (scrollingBackTitle <= -backgroundTitle.width) scrollingBackTitle = 0;
                
                mousePoint = GetMousePosition();
                btnAction = false;

                if (CheckCollisionPointRec(mousePoint, btnBounds))  // Check button state
                {
                    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) btnState = 2;
                    else btnState = 1;

                    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) btnAction = true;
                }
                else btnState = 0;

                if (btnAction)  // apertar no botão para ir para tela ENDING
                {
                    PlaySound(audioEleGosta);
                    currentScreen = START;
                }

                sourceRec.y = btnState*frameHeight; // Calculate button frame rectangle to draw depending on button state

                // apertar enter ou tocar na janela para ir para tela START
                // if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)) currentScreen = START;
            } break;
            case START:
            {
                // nome
                if (CheckCollisionPointRec(GetMousePosition(), textBox)) mouseOnText = true;
                else mouseOnText = false;

                if (mouseOnText)
                {
                    SetMouseCursor(MOUSE_CURSOR_IBEAM); // Set the window's cursor to the I-Beam
                    int keyN = GetCharPressed(); // Get char pressed (unicode character) on the queue

                    while (keyN > 0) // Check if more characters have been pressed on the same frame
                    {
                        if ((keyN >= 32) && (keyN <= 125) && (letterCountName < MAX_INPUT_CHARS)) // NOTE: Only allow keys in range [32..125]
                        {
                            name[letterCountName] = (char)keyN;
                            name[letterCountName+1] = '\0'; // Add null terminator at the end of the string.
                            letterCountName++;
                        }
                        keyN = GetCharPressed(); // Check next character in the queue
                    }

                    if (IsKeyPressed(KEY_BACKSPACE))
                    {
                        letterCountName--;
                        if (letterCountName < 0) letterCountName = 0;
                        name[letterCountName] = '\0';
                    }
                }
                else SetMouseCursor(MOUSE_CURSOR_DEFAULT);

                if (mouseOnText) framesCounter++;
                else framesCounter = 0;
                
                // ana ou caio
                if (CheckCollisionPointRec(GetMousePosition(), selectorBox)) mouseOnSelector = true;
                else mouseOnSelector = false;

                if (mouseOnSelector)
                {
                    SetMouseCursor(MOUSE_CURSOR_IBEAM); // Set the window's cursor to the I-Beam
                    int keyS = GetCharPressed(); // Get char pressed (unicode character) on the queue

                    while (keyS > 0) // Check if more characters have been pressed on the same frame
                    {
                        if ((keyS >= 32) && (keyS <= 125) && (letterCountSelector < MAX_INPUT_CHARS)) // NOTE: Only allow keys in range [32..125]
                        {
                            selected[letterCountSelector] = (char)keyS;
                            selected[letterCountSelector+1] = '\0'; // Add null terminator at the end of the string.
                            letterCountSelector++;
                        }
                        keyS = GetCharPressed(); // Check next character in the queue
                    }

                    if (IsKeyPressed(KEY_BACKSPACE))
                    {
                        letterCountSelector--;
                        if (letterCountSelector < 0) letterCountSelector = 0;
                        name[letterCountSelector] = '\0';
                    }
                }
                else SetMouseCursor(MOUSE_CURSOR_DEFAULT);

                if (mouseOnSelector) framesCounter++;
                else framesCounter = 0;
                
                // botão
                
                mousePoint = GetMousePosition();
                btnAction = false;

                if (CheckCollisionPointRec(mousePoint, btnBounds))  // Check button state
                {
                    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) btnState = 2;
                    else btnState = 1;

                    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) btnAction = true;
                }
                else btnState = 0;

                if (btnAction)  // apertar no botão para ir para tela ENDING
                {
                    PlaySound(audioVaiDarNamoro);
                    currentScreen = HIGH_SCORE;
                }

                sourceRec.y = btnState*frameHeight; // Calculate button frame rectangle to draw depending on button state
                
                // apertar enter janela para ir para tela ENDING
                // if (IsKeyPressed(KEY_ENTER)) currentScreen = HIGH_SCORE;
            } break;
            case HIGH_SCORE:
            {
                // variaveis da tela HIGH_SCORE
                if (IsKeyPressed(KEY_R))
                {
                    score = GetRandomValue(1000, 2000);
                    hiscore = GetRandomValue(2000, 4000);
                }
                
                if (IsKeyPressed(KEY_ENTER))
                {
                    SaveStorageValue(STORAGE_POSITION_SCORE, score);
                    SaveStorageValue(STORAGE_POSITION_HISCORE, hiscore);
                }
                else if (IsKeyPressed(KEY_SPACE))
                {
                    // NOTE: If requested position could not be found, value 0 is returned
                    score = LoadStorageValue(STORAGE_POSITION_SCORE);
                    hiscore = LoadStorageValue(STORAGE_POSITION_HISCORE);
                }
                
                // apertar enter ou tocar na janela para ir para tela ENDING
                if (IsKeyPressed(KEY_N)) currentScreen = ENDING;
            } break;
            case JOGO: // falta ajustar as entradas e os prints das perguntas (nas funções anexas)
            {
                if (strcmp(selected, "C") == 0 || strcmp(selected, "c") == 0 || strcmp(selected, "Caio") == 0 || strcmp(selected, "caio") == 0)
                {
                    carregaRankingDeArquivo(rankingCaio, &numJogadoresCaio, "rankingCaio.txt");
                    lerPerguntasRespostas(perguntas, respostas, "perguntasCaio.txt", "respostaCaio.txt", &numPerguntas);
                }
                else
                {
                    carregaRankingDeArquivo(rankingAna, &numJogadoresAna, "rankingAna.txt");
                    lerPerguntasRespostas(perguntas, respostas, "perguntasAna.txt", "respostaAna.txt", &numPerguntas);
                }

                jogo(perguntas, respostas, numPerguntas, &pontuacao);

                if (strcmp(selected, "C") == 0 || strcmp(selected, "c") == 0 || strcmp(selected, "Caio") == 0 || strcmp(selected, "caio") == 0)
                {
                    atualizaRanking(rankingCaio, name, pontuacao, &numJogadoresCaio);
                    insertionSort(rankingCaio, numJogadoresCaio);
                    salvaRankingEmArquivo(rankingCaio, numJogadoresCaio, "rankingCaio.txt");
                } else {
                    atualizaRanking(rankingAna, name, pontuacao, &numJogadoresAna);
                    insertionSort(rankingAna, numJogadoresAna);
                    salvaRankingEmArquivo(rankingAna, numJogadoresAna, "rankingAna.txt");
                }

                if (strcmp(selected, "C") == 0 || strcmp(selected, "c") == 0 || strcmp(selected, "Caio") == 0 || strcmp(selected, "caio") == 0)
                {
                    for (int i = 0; i < numJogadoresCaio; i++) printf("%d. %s - %d pontos\n", i + 1, rankingCaio[i].nome, rankingCaio[i].pontuacao);
                }
                else
                {
                    for (int i = 0; i < numJogadoresAna; i++) printf("%d. %s - %d pontos\n", i + 1, rankingAna[i].nome, rankingAna[i].pontuacao);
                }
                
                for (int i = 0; i < numPerguntas; i++)
                {
                    free(perguntas[i]);
                    free(respostas[i]);
                }
            } break;
            case ENDING:
            {
                // variaveis da tela ENDING

                // apertar enter ou tocar na janela para voltar para tela TITLE
                if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)) currentScreen = TITLE;
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
                    // desenhar tela LOGO
                    DrawTextureEx(backgroundLogo, (Vector2){ scrollingBackLogo, 0 }, 0.0f, 1.0f, WHITE);
                    DrawTextureEx(backgroundLogo, (Vector2){ (backgroundLogo.width) + scrollingBackLogo, 0 }, 0.0f, 1.0f, WHITE);
                    DrawText("VAI DAR NAMORO - CESAR´S VERSION", 10, 10, 20, RED);
                    DrawText("(c) Cana Productions", screenWidth - 150, screenHeight - 20, 10, RAYWHITE);
                    PlaySound(audioIra);
                } break;
                case TITLE:
                {
                    // desenhar tela TITLE
                    DrawTextureEx(backgroundTitle, (Vector2){ scrollingBackTitle, 0 }, 0.0f, 1.0f, WHITE);
                    DrawTextureEx(backgroundTitle, (Vector2){ (backgroundTitle.width) + scrollingBackTitle, 0 }, 0.0f, 1.0f, WHITE);

                    DrawTextEx(font1, msg1, fontPosition1, (float)font1.baseSize, -3, WHITE);   // Font loading
                    DrawTextEx(font2, msg2, fontPosition2, (float)font2.baseSize, -2, WHITE);   // Font loading
                    DrawTextEx(font3, msg3, fontPosition3, (float)font3.baseSize, 2, WHITE);    // Font loading
                    
                    DrawTextureRec(button, sourceRec, (Vector2){ btnBounds.x, btnBounds.y }, WHITE); // Draw button frame
                } break;
                case START:
                {
                    // desenhar tela START
                    ClearBackground(RAYWHITE);  // falta fazer o fundo bonitinho
                    
                    // nome
                    DrawText("INSIRA SEU NOME:", 200, 140, 20, GRAY);
                    DrawRectangleRec(textBox, LIGHTGRAY);
                    if (mouseOnText) DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, RED);
                    else DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, DARKGRAY);
                    DrawText(name, (int)textBox.x + 5, (int)textBox.y + 8, 40, MAROON);
                    DrawText(TextFormat("INPUT CHARS: %i/%i", letterCountName, MAX_INPUT_CHARS), 315, 250, 15, DARKGRAY);
                    if (mouseOnText)
                    {
                        if (letterCountName < MAX_INPUT_CHARS)
                        {
                            if (((framesCounter/20)%2) == 0) DrawText("_", (int)textBox.x + 8 + MeasureText(name, 40), (int)textBox.y + 12, 40, MAROON);    // Draw blinking underscore char
                        }
                        else DrawText("Press BACKSPACE to delete chars...", 230, 300, 20, GRAY);
                    }
                    
                    // seletor
                    DrawText("Deseja conquistar Ana (A) ou Caio (C):", 200, 330, 20, GRAY);
                    DrawRectangleRec(selectorBox, LIGHTGRAY);
                    if (mouseOnSelector) DrawRectangleLines((int)selectorBox.x, (int)selectorBox.y, (int)selectorBox.width, (int)selectorBox.height, RED);
                    else DrawRectangleLines((int)selectorBox.x, (int)selectorBox.y, (int)selectorBox.width, (int)selectorBox.height, DARKGRAY);
                    DrawText(selected, (int)selectorBox.x + 5, (int)selectorBox.y + 8, 40, MAROON);
                    DrawText(TextFormat("INPUT CHARS: %i/%i", letterCountSelector, MAX_INPUT_CHARS), 315, 430, 15, DARKGRAY);
                    if (mouseOnSelector)
                    {
                        if (letterCountSelector < MAX_INPUT_CHARS)
                        {
                            if (((framesCounter/20)%2) == 0) DrawText("_", (int)selectorBox.x + 8 + MeasureText(selected, 40), (int)selectorBox.y + 12, 40, MAROON);    // Draw blinking underscore char
                        }
                        else DrawText("Press BACKSPACE to delete chars...", 230, 300, 20, GRAY);
                    }
                    
                    // botão
                    DrawTextureRec(button, sourceRec, (Vector2){ btnBounds.x, btnBounds.y }, WHITE); // Draw button frame
                    
                    // PlaySound(audioVaiDarNamoro);
                } break;
                case HIGH_SCORE:
                {
                    // desenhar tela HIGH_SCORE
                    // a tela ta desconfigurada, com textos em cima do outro
                    ClearBackground(RAYWHITE);
                    DrawText("HIGH SCORE SCREEN", 20, 40, 40, MAROON);
                    DrawText("PRESS N to JUMP to ENDING SCREEN", 20, 280, 20, MAROON);
                    
                    DrawText(TextFormat("NAME: %s", name), 350, 60, 40, MAROON);
                    DrawText(TextFormat("SCORE: %i", score), 280, 130, 40, MAROON);
                    DrawText(TextFormat("HI-SCORE: %i", hiscore), 210, 200, 50, BLACK);

                    DrawText(TextFormat("frames: %i", framesCounter), 10, 10, 20, LIME);

                    DrawText("Press R to generate random numbers", 220, 100, 20, LIGHTGRAY);
                    DrawText("Press ENTER to SAVE values", 250, 310, 20, LIGHTGRAY);
                    DrawText("Press SPACE to LOAD values", 252, 350, 20, LIGHTGRAY);
                    
                    PlaySound(audioIra);
                } break;
                case JOGO:
                {
                    
                } break;
                case ENDING:
                {
                    // desenhar tela ENDING
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

    // Desinicialização
    //--------------------------------------------------------------------------------------

    // unload de todo dado carregado (textura, fonte, audio)
    
        // unload audio
        UnloadSound(audioVaiDarNamoro);
        
        // unload tela LOGO
        UnloadTexture(backgroundLogo);  // Unload background texture
        
        // unload tela TITLE
        UnloadFont(font1);      // Font unloading
        UnloadFont(font2);      // Font unloading
        UnloadFont(font3);      // Font unloading
        UnloadTexture(backgroundTitle);  // Unload background texture
        
        // unload tela START
        UnloadTexture(button);  // Unload button texture
    
        // unload tela ENDING
    
    CloseAudioDevice();
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

void atualizaRanking(Jogador ranking[], char *nome, int pontuacao, int *numJogadores)
{
    int i;
    for (i = 0; i < *numJogadores; ++i) {
        if (strcmp(ranking[i].nome, nome) == 0) {
            ranking[i].pontuacao = pontuacao;
            return;
        }
    }
    strcpy(ranking[i].nome, nome);
    ranking[i].pontuacao = pontuacao;
    (*numJogadores)++;
}

void insertionSort(Jogador ranking[], int numJogadores)
{
    int i, j;
    Jogador chave;
    for (i = 1; i < numJogadores; i++) {
        chave = ranking[i];
        j = i - 1;

        while (j >= 0 && ranking[j].pontuacao < chave.pontuacao) {
            ranking[j + 1] = ranking[j];
            j = j - 1;
        }
        ranking[j + 1] = chave;
    }
}

void salvaRankingEmArquivo(Jogador ranking[], int numJogadores, const char *nomeArquivo)
{
    FILE *file = fopen(nomeArquivo, "w+");
    if (file == NULL) {
        perror("Erro ao abrir arquivo de ranking");
        return;
    }

    for (int i = 0; i < numJogadores; i++) {
        fprintf(file, "%s %d\n", ranking[i].nome, ranking[i].pontuacao);
    }

    fclose(file);
}

void carregaRankingDeArquivo(Jogador ranking[], int *numJogadores, const char *nomeArquivo)
{
    FILE *file = fopen(nomeArquivo, "r");
    if (file == NULL) {
        perror("Erro ao abrir arquivo de ranking");
        *numJogadores = 0;
        return;
    }

    *numJogadores = 0;
    while (fscanf(file, "%49s %d", ranking[*numJogadores].nome, &ranking[*numJogadores].pontuacao) == 2) {
        (*numJogadores)++;
    }

    fclose(file);
}

void lerPerguntasRespostas(char *perguntas[], char *respostas[], const char *perguntasJogo, const char *respostaJogo, int *tamanho)
{
    FILE *fpPerguntas = fopen(perguntasJogo, "r");
    FILE *fpRespostas = fopen(respostaJogo, "r");
    char linha[1000];
    *tamanho = 0;

    if (fpPerguntas == NULL || fpRespostas == NULL) {
        printf("Erro ao abrir arquivos.\n");
        return;
    }

    while (fgets(linha, sizeof(linha), fpPerguntas)) {
        perguntas[*tamanho] = strdup(linha);
        fgets(linha, sizeof(linha), fpRespostas);
        linha[strcspn(linha, "\n")] = 0;
        respostas[*tamanho] = strdup(linha);
        (*tamanho)++;
    }

    fclose(fpPerguntas);
    fclose(fpRespostas);
}

RespostaNode* addRedposta(RespostaNode *head, char *resposta)
{
    RespostaNode *novoNode = (RespostaNode*)malloc(sizeof(RespostaNode));
    strcpy(novoNode->resposta, resposta);
    novoNode->proximo = NULL;

    if (head == NULL) {
        return novoNode;
    }

    RespostaNode *atual = head;
    while (atual->proximo != NULL) {
        atual = atual->proximo;
    }
    atual->proximo = novoNode;

    return head;
}

void liberaLista(RespostaNode *head)
{
    RespostaNode *atual = head;
    while (atual != NULL) {
        RespostaNode *temp = atual;
        atual = atual->proximo;
        free(temp);
    }
}

void jogo(char *perguntas[], char *respostas[], int numPerguntas, int *pontuacao)
{
    char respostaUsuario[1000];

    for (int i = 0; i < numPerguntas; i++) {
        printf("Pergunta %d: %s\n", i + 1, perguntas[i]);
        printf("Resposta: ");
        fgets(respostaUsuario, 1000, stdin); // Modificação aqui
        respostaUsuario[strcspn(respostaUsuario, "\n")] = 0;

        respostas[i][strcspn(respostas[i], "\n")] = 0;

        if (strcasecmp(respostaUsuario, respostas[i]) == 0) {
            (*pontuacao)++;
        }
    }

    printf("Você acertou %d de %d perguntas.\n", *pontuacao, numPerguntas);
}

// Save integer value to storage file (to defined position)
// NOTE: Storage positions is directly related to file memory layout (4 bytes each integer)
bool SaveStorageValue(unsigned int position, int value)
{
    bool success = false;
    int dataSize = 0;
    unsigned int newDataSize = 0;
    unsigned char *fileData = LoadFileData(STORAGE_DATA_FILE, &dataSize);
    unsigned char *newFileData = NULL;

    if (fileData != NULL)
    {
        if (dataSize <= (position*sizeof(int)))
        {
            // Increase data size up to position and store value
            newDataSize = (position + 1)*sizeof(int);
            newFileData = (unsigned char *)RL_REALLOC(fileData, newDataSize);

            if (newFileData != NULL)
            {
                // RL_REALLOC succeded
                int *dataPtr = (int *)newFileData;
                dataPtr[position] = value;
            }
            else
            {
                // RL_REALLOC failed
                TraceLog(LOG_WARNING, "FILEIO: [%s] Failed to realloc data (%u), position in bytes (%u) bigger than actual file size", STORAGE_DATA_FILE, dataSize, position*sizeof(int));

                // We store the old size of the file
                newFileData = fileData;
                newDataSize = dataSize;
            }
        }
        else
        {
            // Store the old size of the file
            newFileData = fileData;
            newDataSize = dataSize;

            // Replace value on selected position
            int *dataPtr = (int *)newFileData;
            dataPtr[position] = value;
        }

        success = SaveFileData(STORAGE_DATA_FILE, newFileData, newDataSize);
        RL_FREE(newFileData);

        TraceLog(LOG_INFO, "FILEIO: [%s] Saved storage value: %i", STORAGE_DATA_FILE, value);
    }
    else
    {
        TraceLog(LOG_INFO, "FILEIO: [%s] File created successfully", STORAGE_DATA_FILE);

        dataSize = (position + 1)*sizeof(int);
        fileData = (unsigned char *)RL_MALLOC(dataSize);
        int *dataPtr = (int *)fileData;
        dataPtr[position] = value;

        success = SaveFileData(STORAGE_DATA_FILE, fileData, dataSize);
        UnloadFileData(fileData);

        TraceLog(LOG_INFO, "FILEIO: [%s] Saved storage value: %i", STORAGE_DATA_FILE, value);
    }

    return success;
}

// Load integer value from storage file (from defined position)
// NOTE: If requested position could not be found, value 0 is returned
int LoadStorageValue(unsigned int position)
{
    int value = 0;
    int dataSize = 0;
    unsigned char *fileData = LoadFileData(STORAGE_DATA_FILE, &dataSize);

    if (fileData != NULL)
    {
        if (dataSize < (position*4)) TraceLog(LOG_WARNING, "FILEIO: [%s] Failed to find storage position: %i", STORAGE_DATA_FILE, position);
        else
        {
            int *dataPtr = (int *)fileData;
            value = dataPtr[position];
        }

        UnloadFileData(fileData);

        TraceLog(LOG_INFO, "FILEIO: [%s] Loaded storage value: %i", STORAGE_DATA_FILE, value);
    }

    return value;
}
