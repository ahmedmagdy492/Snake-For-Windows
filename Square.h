#pragma once

#include <stdlib.h>
#include <raylib.h>
#define SQUARE_LEN 25
#define MAX_SNAKE_LEN 100

static int speedX = 0, speedY = 0;
static int score = 0;

struct Direction {
    int direction;
    char axis;
};

struct Square {
    struct Direction cur_dir;
    struct Direction prev_dir;
    int x;
    int y;
    int w;
    int h;
    Color color;
    struct Square* next;
    struct Square* prev;
};

struct DLinkedList {
    struct Square* head;
    struct Square* tail;
};

static struct DLinkedList snake = { NULL, NULL };

static int GetNodesCount();

void Append(struct Square* square) {
    if (GetNodesCount() == MAX_SNAKE_LEN)
        return;

    if (snake.head == NULL && snake.tail == NULL) {
        snake.head = snake.tail = square;
    }
    else {
        square->x = snake.tail->x;
        square->y = snake.tail->y;
        square->color = YELLOW;
        snake.tail->next = square;
        square->prev = snake.tail;
        snake.tail = snake.tail->next;
    }
    square->cur_dir.direction = square->prev_dir.direction = 1;
    square->cur_dir.axis = square->prev_dir.axis = 'x';
}

void ClearLinkedList() {
    struct Square* ptr = snake.head;

    while (ptr != NULL) {
        struct Square* temp = NULL;
        if (ptr) {
            temp = ptr->next;
            free(ptr);
        }
        ptr = temp;
    }

    snake.head = NULL;
    snake.tail = NULL;
}

void DrawSnake() {
    struct Square* ptr = snake.head;

    while (ptr != NULL) {
        DrawRectangle(ptr->x, ptr->y, ptr->w, ptr->h, ptr->color);
        ptr = ptr->next;
    }
}

int Move(struct Direction* dir, int sw, int sh) {
    struct Square* ptr = snake.head;
    struct Direction temp;

    temp.direction = ptr->cur_dir.direction;
    temp.axis = ptr->cur_dir.axis;

    ptr->cur_dir.direction = dir->direction;
    ptr->cur_dir.axis = dir->axis;

    ptr->prev_dir.direction = temp.direction;
    ptr->prev_dir.axis = temp.axis;
    int prevX = ptr->x;
    int prevY = ptr->y;

    if (ptr->cur_dir.axis == 'x') {
        speedX = SQUARE_LEN;
        speedY = 0;
        ptr->x += (speedX * ptr->cur_dir.direction);
        if (ptr->x > sw) return -1;
        else if (ptr->x < 0) return -1;
    }
    else if (ptr->cur_dir.axis == 'y') {
        speedX = 0;
        speedY = SQUARE_LEN;
        ptr->y += (speedY * ptr->cur_dir.direction);
        if (ptr->y > sh) ptr->y = 0;
        else if (ptr->y < 0) ptr->y = sh;
    }

    ptr = ptr->next;

    while (ptr != NULL) {
        ptr->prev_dir.direction = ptr->cur_dir.direction;
        ptr->prev_dir.axis = ptr->cur_dir.axis;
        ptr->cur_dir.direction = temp.direction;
        ptr->cur_dir.axis = temp.axis;
        temp.direction = ptr->prev_dir.direction;
        temp.axis = ptr->prev_dir.axis;

        if (ptr->cur_dir.axis == 'x') {
            int tempX = ptr->x;
            ptr->x = prevX;
            prevX = tempX;
        }
        else if (ptr->cur_dir.axis == 'y') {
            int tempY = ptr->y;
            ptr->y = prevY;
            prevY = tempY;
        }
        ptr = ptr->next;
    }

    return 1;
}

void SetPlayerScore(int _score) {
    score = _score;
}

void ResetScore() {
    score = 0;
}

int GetPlayerScore() {
    return score;
}

static int GetNodesCount() {
    int count = 0;
    struct Square* ptr = snake.head;

    while (ptr != NULL) {
        ++count;
        ptr = ptr->next;
    }

    return count;
}

int HasCollided(struct Square* head, struct Square* ptr) {
    return (head->x == ptr->x) && (head->y == ptr->y);
}

int AmICollidingWithMySelf() {
    struct Square* head = snake.head;
    struct Square* ptr = snake.tail;
    int i = 0;

    while (ptr != NULL) {
        if (i >= 2) {
            if (ptr != head)
                if (HasCollided(head, ptr)) return 1;
        }
        ptr = ptr->prev;
        ++i;
    }

    return 0;
}