#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include "Square.h"
#include "Apple.h"
#include "Helper.h"
#include "Common.h"

#define SCORE_LEN 1000

enum GameMode {
    GameOver,
    Playing,
    MenuScreen
};

int timer = 0;
int framesCounter = 0;
int hasPlayerStarted = 0;
enum GameMode currentGameMode = MenuScreen;
int prevKey = -1;
int isPaused = 0;
int allowMove = 0;
char scoreText[SCORE_LEN];

void RandomizeApplePosition() {
    int nextMultipleOf5Width = GetNextMultipleOf(25, SCREEN_WIDTH);
    int nextMultipleOf5Height = GetNextMultipleOf(25, SCREEN_HEIGHT);
    int x = GetRandNumber(SCREEN_WIDTH - GetAppleWidth()) % nextMultipleOf5Width;
    int y = GetRandNumber(SCREEN_HEIGHT - GetAppleHeight()) % nextMultipleOf5Height;
    SetApplePos(x, y);
}

struct Square* ResetGame() {
    ClearLinkedList();
    currentGameMode = Playing;
    SetPlayerScore(0);
    timer = 0;
    prevKey = -1;
    allowMove = 0;
    framesCounter = 0;
    memset(scoreText, 0, SCORE_LEN);
    RandomizeApplePosition();
    hasPlayerStarted = 0;

    struct Square* square = (struct Square*)malloc(sizeof(struct Square));
    square->x = 100;
    square->y = 100;
    square->w = square->h = SQUARE_LEN;
    square->color = ORANGE;
    square->next = NULL;
    square->prev = NULL;
    Append(square);
    return square;
}

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Snake");
    InitAudioDevice();
    Image bg = LoadImage("resources/menu-bg.png");
    Texture2D bgTexture = LoadTextureFromImage(bg);

    struct Square* square = (struct Square*)malloc(sizeof(struct Square));
    square->x = 100;
    square->y = 100;
    square->w = square->h = SQUARE_LEN;
    square->color = ORANGE;
    square->next = NULL;
    square->prev = NULL;
    Append(square);

    SetTargetFPS(60);

    Sound music = LoadSound("resources/scoreplay.ogg");
    Sound fail = LoadSound("resources/fail.ogg");

    struct Direction dir = {
      1,
      'x'
    };
    SetExitKey(KEY_Q);

    while (!WindowShouldClose()) {
        if (currentGameMode == Playing) {
            if (IsKeyPressed(KEY_P)) isPaused = isPaused == 1 ? 0 : 1;

            if (!isPaused) {
                if (IsKeyPressed(KEY_RIGHT)) {
                    if (prevKey != KEY_LEFT) {
                        if (!hasPlayerStarted) {
                            hasPlayerStarted = 1;
                        }
                        prevKey = KEY_RIGHT;
                        dir.direction = 1;
                        dir.axis = 'x';
                        allowMove = 0;
                    }
                }
                if (IsKeyPressed(KEY_LEFT)) {
                    if (prevKey != KEY_RIGHT) {
                        if (!hasPlayerStarted) {
                            hasPlayerStarted = 1;
                        }
                        prevKey = KEY_LEFT;
                        dir.direction = -1;
                        dir.axis = 'x';
                        allowMove = 0;
                    }
                }
                if (IsKeyPressed(KEY_UP)) {
                    if (prevKey != KEY_DOWN) {
                        if (!hasPlayerStarted) {
                            hasPlayerStarted = 1;
                        }
                        prevKey = KEY_UP;
                        dir.direction = -1;
                        dir.axis = 'y';
                        allowMove = 0;
                    }
                }
                if (IsKeyPressed(KEY_DOWN)) {
                    if (prevKey != KEY_UP) {
                        if (!hasPlayerStarted) {
                            hasPlayerStarted = 1;
                        }
                        prevKey = KEY_DOWN;
                        dir.direction = 1;
                        dir.axis = 'y';
                    }
                }

                if (IsKeyPressed(KEY_ESCAPE)) {
                    square = ResetGame();
                    currentGameMode = MenuScreen;
                }

                if (hasPlayerStarted) {
                    if ((framesCounter % 3) == 0) {
                        int moveResult = Move(&dir, SCREEN_WIDTH, SCREEN_HEIGHT);
                        if (moveResult == -1) {
                            currentGameMode = GameOver;
                            PlaySound(fail);
                        }
                    }
                }

                BeginDrawing();
                ClearBackground(DARKGREEN);

                DrawSnake();
                DrawApple();

                IntToAsci(GetPlayerScore(), scoreText);
                DrawText(scoreText, (SCREEN_WIDTH - 20) / 2, 10, 40, WHITE);

                if (AmICollidingWithMySelf()) {
                    currentGameMode = GameOver;
                    PlaySound(fail);
                }

                if (HasPlayerCollidedWithApple(square->x, square->y, square->w, square->h)) {
                    PlaySound(music);
                    SetPlayerScore(GetPlayerScore() + 1);
                    RandomizeApplePosition();

                    struct Square* newSquare = (struct Square*)malloc(sizeof(struct Square));
                    newSquare->w = newSquare->h = SQUARE_LEN;
                    newSquare->next = NULL;
                    newSquare->prev = NULL;
                    Append(newSquare);
                }

                if (timer == APPLE_CHANGE_POS_TIME) {
                    timer = 0;
                    RandomizeApplePosition();
                }

                EndDrawing();

                ++timer;
                ++framesCounter;
            }
            else {
                BeginDrawing();
                DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, CLITERAL(Color) { 0, 0, 0, 255 });
                DrawText("Pause", (SCREEN_WIDTH - 500) / 2, (SCREEN_HEIGHT - 120) / 2, 120, WHITE);
                EndDrawing();
            }
        }
        else if (currentGameMode == GameOver) {
            // if game is over
            BeginDrawing();
            DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, CLITERAL(Color) { 0, 0, 0, 255 });
            DrawText("Game Over", (SCREEN_WIDTH - 500) / 2, (SCREEN_HEIGHT - 120) / 2, 120, WHITE);
            IntToAsci(GetPlayerScore(), scoreText);
            DrawText("Score: ", (SCREEN_WIDTH - 500) / 2, (SCREEN_HEIGHT - 80) / 2 + 120, 80, WHITE);
            DrawText(scoreText, (SCREEN_WIDTH) / 2 + 100, (SCREEN_HEIGHT - 80) / 2 + 120, 80, WHITE);

            if (IsKeyPressed(KEY_ENTER)) {
                square = ResetGame();
            }
            EndDrawing();
        }
        else if (currentGameMode == MenuScreen) {

            BeginDrawing();
            ClearBackground(DARKGREEN);
            DrawTexture(bgTexture, (SCREEN_WIDTH - bgTexture.width) / 2, 0, WHITE);
            int titleFontHeight = 120;
            int titleWidth = MeasureText("Snake Game", titleFontHeight);

            DrawText("Snake Game", (SCREEN_WIDTH - titleWidth) / 2, 200, titleFontHeight, WHITE);

            int fontHeight = 24;
            int textWidth = MeasureText("Start Game", fontHeight);
            int btnWidth = textWidth + 20, btnHeight = fontHeight + 20;
            int btnPosX = (SCREEN_WIDTH - btnWidth) / 2, btnPosY = (SCREEN_HEIGHT - btnHeight) / 2;
            DrawRectangle(btnPosX, btnPosY, btnWidth, btnHeight, BLACK);

            int textPositionX = btnPosX + ((btnWidth - textWidth) / 2);
            int textPositionY = btnPosY + ((btnHeight - fontHeight) / 2);
            DrawText("Start Game", textPositionX, textPositionY, fontHeight, WHITE);

            DrawText("Made by Ahmed Magdy", 10, SCREEN_HEIGHT - fontHeight - 10, fontHeight, WHITE);
            EndDrawing();

            if (IsMouseButtonPressed(0)) {
                Vector2 mousePos = GetMousePosition();
                if ((mousePos.x >= btnPosX && mousePos.x <= (btnWidth + btnPosX)) &&
                    (mousePos.y >= btnPosY && mousePos.y <= (btnHeight + btnPosY))) {
                    currentGameMode = Playing;
                }
            }
        }
    }

    UnloadImage(bg);

    UnloadSound(music);
    UnloadSound(fail);
    CloseAudioDevice();

    CloseWindow();

    ClearLinkedList();

    return 0;
}