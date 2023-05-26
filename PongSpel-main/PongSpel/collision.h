#ifndef COLLISION_H
#define COLLISION_H


#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "application.h"
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <SDL2/SDL_mixer.h>
#include "ball.h"
#include "paddle.h"
#include "counter.h"




    
bool check_collision(Ball *ball, Paddle *paddle, Paddle *paddle2, Paddle *paddle3, Paddle *paddle4, Player* all_players_info, SDL_Renderer *renderer, TTF_Font *font, SDL_Window *window, Mix_Chunk* SoundEffect);








#endif