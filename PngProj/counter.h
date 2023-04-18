#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "stdbool.h"

#ifndef COUNTER_H
#define COUNTER_H

#define WINDOW_WIDTH 400

typedef struct Score {
    int player1;
    int player2;
} Score;

void increaseScore(int player, Score* score); // Funktion för att öka poängen
void drawScore(SDL_Renderer* renderer, TTF_Font* font, Score score);
#endif