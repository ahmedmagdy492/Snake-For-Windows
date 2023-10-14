#pragma once
#include <raylib.h>

#define NPIXELS 10

struct Pixel {
  double x;
  double y;
  int w;
  int h;
  Color color;
};

struct Pixel pixels[NPIXELS] = {
  {10.0, 10.0, 10, 10, ORANGE}, {20.0, 10.0, 10, 10, ORANGE}, {30.0, 10.0, 10, 10, ORANGE}, {40.0, 10.0, 10, 10, ORANGE}, {50.0, 10.0, 10, 10, ORANGE}, {10.0, 20.0, 10, 10, ORANGE}, {20.0, 20.0, 10, 10, ORANGE}, {30.0, 20.0, 10, 10, ORANGE}, {40.0, 20.0, 10, 10, ORANGE}, {50.0, 20.0, 10, 10, ORANGE}
};

struct Direction {
  char axis;
  int direction;
};

struct Player {
  double speed;
  struct Direction direction;
};

void DrawSnake() {
  int i;
  
  for(i = 0; i < NPIXELS; ++i) {
    DrawRectangle(pixels[i].x, pixels[i].y, pixels[i].w, pixels[i].h, pixels[i].color);
  }
}

void Move(struct Player *player) {
  int i;

  for(i = 0; i < NPIXELS; ++i) {
    if(player->direction.axis == 'x') {
      pixels[i].x += (player->speed * player->direction.direction);
    }
    else if(player->direction.axis == 'y') {
      pixels[i].y += (player->speed * player->direction.direction);
    }
  }
}
