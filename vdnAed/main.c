#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "raylib.h"

typedef enum GameScreen { LOGO = 0, TITLE, GAMEPLAY, ENDING } GameScreen;

typedef struct respostaNode {
    char resposta[1000];
    struct respostaNode *proximo;
} RespostaNode;

typedef struct {
    char nome[50];
    int pontuacao;
} Jogador;

// Declarações de Funções
int mainJogo(int x); // é a main do jogo, falta integrar com o raylib
void atualizaRanking(Jogador ranking[], char *nome, int pontuacao, int *numJogadores);
void insertionSort(Jogador ranking[], int numJogadores);
void salvaRankingEmArquivo(Jogador ranking[], int numJogadores, const char *nomeArquivo);
void carregaRankingDeArquivo(Jogador ranking[], int *numJogadores, const char *nomeArquivo);
void lerPerguntasRespostas(char *perguntas[], char *respostas[], const char *perguntasJogo, const char *respostaJogo, int *tamanho);
RespostaNode* addRedposta(RespostaNode *head, char *resposta);
void liberaLista(RespostaNode *head);
void jogo(char *perguntas[], char *respostas[], int numPerguntas, int *pontuacao);

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
    char nome[50];
    char escolha;
    int pontuacao = 0;
    int numPerguntas = 0;
    char *perguntas[100];
    char *respostas[100];
    Jogador rankingAna[100];
    Jogador rankingCaio[100];
    int numJogadoresAna = 0;
    int numJogadoresCaio = 0;
    
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

                // apertar enter ou tocar na janela para ir para tela GAMEPLAY
                if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)) currentScreen = GAMEPLAY;
            } break;
            case GAMEPLAY:
            {
                // variaveis da tela GAMEPLAY

                // apertar enter ou tocar na janela para ir para tela ENDING
                if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)) currentScreen = ENDING;
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
                    PlaySound(audioEleGosta);
                } break;
                case GAMEPLAY:
                {
                    // desenhar tela GAMEPLAY
                    DrawRectangle(0, 0, screenWidth, screenHeight, PURPLE);
                    DrawText("GAMEPLAY SCREEN", 20, 20, 40, MAROON);
                    DrawText("PRESS ENTER or TAP to JUMP to ENDING SCREEN", 130, 220, 20, MAROON);
                    PlaySound(audioIra);
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
        
        // unload tela GAMEPLAY
        // unload tela ENDING
    
    CloseAudioDevice();
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

// falta juntar a logica com o front
int mainJogo(int x)
{
    char nome[50];
    char escolha;
    int pontuacao = 0;
    int numPerguntas = 0;
    char *perguntas[100];
    char *respostas[100];
    Jogador rankingAna[100];
    Jogador rankingCaio[100];
    int numJogadoresAna = 0;
    int numJogadoresCaio = 0;

    printf("Digite seu nome: ");
    scanf("%49s", nome);
    getchar();
    printf("Quem você quer conquistar? (Caio = C, Ana = A): ");
    scanf(" %c", &escolha);
    getchar();
    
    if (escolha == 'C' || escolha == 'c') {
        carregaRankingDeArquivo(rankingCaio, &numJogadoresCaio, "rankingCaio.txt");
        lerPerguntasRespostas(perguntas, respostas, "perguntasCaio.txt", "respostaCaio.txt", &numPerguntas);
    } else {
        carregaRankingDeArquivo(rankingAna, &numJogadoresAna, "rankingAna.txt");
        lerPerguntasRespostas(perguntas, respostas, "perguntasAna.txt", "respostaAna.txt", &numPerguntas);
    }

    jogo(perguntas, respostas, numPerguntas, &pontuacao);

    if (escolha == 'C' || escolha == 'c') {
        atualizaRanking(rankingCaio, nome, pontuacao, &numJogadoresCaio);
        insertionSort(rankingCaio, numJogadoresCaio);
        salvaRankingEmArquivo(rankingCaio, numJogadoresCaio, "rankingCaio.txt");
    } else {
        atualizaRanking(rankingAna, nome, pontuacao, &numJogadoresAna);
        insertionSort(rankingAna, numJogadoresAna);
        salvaRankingEmArquivo(rankingAna, numJogadoresAna, "rankingAna.txt");
    }

    if (escolha == 'C' || escolha == 'c') {
    for (int i = 0; i < numJogadoresCaio; i++) {
        printf("%d. %s - %d pontos\n", i + 1, rankingCaio[i].nome, rankingCaio[i].pontuacao);
    }
    } else {
        for (int i = 0; i < numJogadoresAna; i++) {
            printf("%d. %s - %d pontos\n", i + 1, rankingAna[i].nome, rankingAna[i].pontuacao);
        }
    }
    for (int i = 0; i < numPerguntas; i++) {
        free(perguntas[i]);
        free(respostas[i]);
    }

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
