#pragma once

#include <raylib.h>

#define APPLE_CHANGE_POS_TIME 1000
#define APPLE_WIDTH 25
#define APPLE_HEIGHT 25

struct Apple {
    int x;
    int y;
    int w;
    int h;
    Color color;
};

struct Apple apple = { 150, 150, APPLE_WIDTH, APPLE_HEIGHT, RED };

void SetApplePos(int x, int y) {
    apple.x = x;
    apple.y = y;
}

void DrawApple() {
    DrawRectangle(apple.x, apple.y, apple.w, apple.h, apple.color);
}

int GetAppleWidth() {
    return apple.w;
}

int GetAppleHeight() {
    return apple.h;
}

int HasPlayerCollidedWithApple(int playerX, int playerY, int playerW, int playerH) {
    // top left point
    if (
        (playerX >= apple.x && playerX <= (apple.x + apple.w)) &&
        (playerY >= apple.y && playerY <= (apple.y + apple.h))) {
        return 1;
    }

    // top right point
    if (
        ((playerX + playerW) >= apple.x && (playerX + playerW) <= (apple.x + apple.w)) &&
        (playerY >= apple.y && playerY <= (apple.y + apple.h))
        ) {
        return 1;
    }

    // bottom right point
    if (
        ((playerX + playerW) >= apple.x && (playerX + playerW) <= (apple.x + apple.w)) &&
        ((playerY + playerH) >= apple.y && (playerY + playerH) <= (apple.y + apple.h))
        ) {
        return 1;
    }

    if ((playerX >= apple.x && playerX <= (apple.x + apple.w)) &&
        ((playerY + playerH) >= apple.y && (playerY + playerH) <= (apple.y + apple.h))
        ) {
        return 1;
    }

    return 0;
}