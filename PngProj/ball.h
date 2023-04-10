#ifndef BALL_H
#define BALL_H

#include <SDL.h>


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
void update_ball(Ball* ball, float delta_time);
void render_ball(Ball* ball, SDL_Renderer* renderer);
void destroy_ball(Ball* ball);

#endif
