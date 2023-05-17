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

Paddle* initialize_paddle(SDL_Renderer* renderer, int x, int y, int width, int height, int index);
void update_paddle(Paddle* paddle, float delta_time, int playerIndex);
void render_paddle(Paddle* paddle, SDL_Renderer* renderer);
void destroy_paddle(Paddle* paddle);

#endif

