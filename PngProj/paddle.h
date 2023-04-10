#include <SDL2/SDL.h>
#include <stdio.h>
#ifndef PADDLE_H
#define PADDLE_H



typedef struct {
    float x;
    float y;
    float width;
    float height;
    float speed;
    SDL_Texture* texture;
} Paddle;

void initialize_paddle(Paddle* paddle, SDL_Renderer* renderer);
void update_paddle(Paddle* paddle, float delta_time);
void render_paddle(Paddle* paddle, SDL_Renderer* renderer);
void destroy_paddle(Paddle* paddle);

#endif

