#ifndef BALL_H
#define BALL_H

#include <SDL2/SDL.h>
#include "paddle.h"

typedef struct {
    float x;
    float y;
    float dx;
    float dy;
    float speed;
    int radius;
    SDL_Texture* texture;
} Ball;

void initialize_ball(Ball* ball, SDL_Renderer* renderer);
void update_ball(Ball* ball, Paddle *paddle,Paddle *paddle2,Paddle *paddle3,Paddle *paddle4,float delta_time);
void render_ball(Ball* ball, SDL_Renderer* renderer);
void destroy_ball(Ball* ball);

#endif
