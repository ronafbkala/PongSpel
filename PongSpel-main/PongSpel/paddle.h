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
    int index;
    SDL_Texture* texture;
} Paddle;

Paddle* initialize_paddle(SDL_Renderer* renderer);
Paddle* initialize_paddle2(SDL_Renderer* renderer);
Paddle* initialize_paddle3(SDL_Renderer* renderer);
Paddle* initialize_paddle4(SDL_Renderer* renderer);
void update_paddle(Paddle* paddle, Paddle* paddle2, Paddle* paddle3, Paddle* paddle4, float delta_time, int playerIndex);
void render_paddle(Paddle* paddle, SDL_Renderer* renderer);
void destroy_paddle(Paddle* paddle, Paddle* paddle2, Paddle* paddle3, Paddle* paddle4);

#endif

