#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include "Square.h"
#include "Apple.h"
#include "Helper.h"

#define SCORE_LEN 1000
#define SQUARE_LEN 25

const int SCREEN_WIDTH = 1600;
const int SCREEN_HEIGHT = 900;
int timer = 0;
int hasPlayerStarted = 0;
int isGameOver = 0;
int prevKey = -1;
char scoreText[SCORE_LEN];

void RandomizeApplePosition() {
    int x = GetRandNumber(SCREEN_WIDTH - GetAppleWidth());
    int y = GetRandNumber(SCREEN_HEIGHT - GetAppleHeight());
    SetApplePos(x, y);
}

struct Square* ResetGame() {
    ClearLinkedList();
    isGameOver = 0;
    SetPlayerScore(0);
    timer = 0;
    prevKey = -1;
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

    struct Direction dir = { 1, 'x' };
    SetExitKey(KEY_Q);

    while (!WindowShouldClose()) {
        if (!isGameOver) {
            if (IsKeyDown(KEY_RIGHT)) {
                if (prevKey != KEY_LEFT) {
                    if (!hasPlayerStarted) {
                        hasPlayerStarted = 1;
                    }
                    prevKey = KEY_RIGHT;
                    dir.direction = 1;
                    dir.axis = 'x';
                }
            }
            else if (IsKeyDown(KEY_LEFT)) {
                if (prevKey != KEY_RIGHT) {
                    if (!hasPlayerStarted) {
                        hasPlayerStarted = 1;
                    }
                    prevKey = KEY_LEFT;
                    dir.direction = -1;
                    dir.axis = 'x';
                }
            }
            else if (IsKeyDown(KEY_UP)) {
                if (prevKey != KEY_DOWN) {
                    if (!hasPlayerStarted) {
                        hasPlayerStarted = 1;
                    }
                    prevKey = KEY_UP;
                    dir.direction = -1;
                    dir.axis = 'y';
                }
            }
            else if (IsKeyDown(KEY_DOWN)) {
                if (prevKey != KEY_UP) {
                    if (!hasPlayerStarted) {
                        hasPlayerStarted = 1;
                    }
                    prevKey = KEY_DOWN;
                    dir.direction = 1;
                    dir.axis = 'y';
                }
            }

            if (hasPlayerStarted) {
                int moveResult = Move(&dir, SCREEN_WIDTH, SCREEN_HEIGHT);
                if (moveResult == -1) {
                    isGameOver = 1;
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
            isGameOver = 1;
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

        if (isGameOver) {
            DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, CLITERAL(Color){ 0, 0, 0, 128 });
            DrawText("Game Over", (SCREEN_WIDTH - 500) / 2, (SCREEN_HEIGHT - 120) / 2, 120, WHITE);
            IntToAsci(GetPlayerScore(), scoreText);
            DrawText("Score: ", (SCREEN_WIDTH - 500) / 2, (SCREEN_HEIGHT - 80) / 2 + 120, 80, WHITE);
            DrawText(scoreText, (SCREEN_WIDTH) / 2 + 100, (SCREEN_HEIGHT - 80) / 2 + 120, 80, WHITE);

            if (IsKeyDown(KEY_ENTER)) {
                square = ResetGame();
            }
        }

        EndDrawing();

        if (!isGameOver) {
            ++timer;
        }
    }

    UnloadSound(music);
    UnloadSound(fail);
    CloseAudioDevice();

    CloseWindow();

    ClearLinkedList();

    return 0;
}