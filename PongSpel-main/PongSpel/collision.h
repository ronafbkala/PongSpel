#ifndef COUNTER_H
#define COUNTER_H


#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "application.h"
#include <SDL2/SDL_ttf.h>
#include "ball.h"
#include "paddle.h"
#include <stdbool.h>



  typedef struct {                       // kan ligga till extra egenskaper for players sen 
    int score;
   /* int lives;                      
    char name[50];
    SDL_Color color;*/
} Player;

    
void initPlayers(Player* all_players_info);
bool check_collision(Ball *ball, Paddle *paddle, Paddle *paddle2, Paddle *paddle3, Paddle *paddle4, Player* all_players_info, SDL_Renderer *renderer, TTF_Font *font, SDL_Window *window);
void printScore(SDL_Renderer *renderer, TTF_Font *font, Player* all_players_info, SDL_Window *window);








#endif