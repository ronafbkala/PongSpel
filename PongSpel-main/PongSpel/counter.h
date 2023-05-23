#ifndef COUNTER_H
#define COUNTER_H

#include <SDL2/SDL.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include "application.h"
#include "ball.h"
#include "collision.h"
#include <stdbool.h>

typedef struct {                      
    int score;
            
   /* char name[50];
    SDL_Color color; */
}  Player;


void initializeScore(Player *all_players_info);
void drawScore(Player *all_players_info,TTF_Font* font, SDL_Renderer *renderer, SDL_Window *window);
int check_paddle_life(Paddle *paddle, int *live_chance);
bool checkWinner(Player* all_players_info, int numPlayers, int* winnerIndex);
void endGame(SDL_Renderer* renderer, TTF_Font* font, SDL_Window* window, int *winnerIndex, SDL_Texture *win_texture);


#endif
