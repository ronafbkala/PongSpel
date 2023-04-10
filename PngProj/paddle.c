#include "paddle.h"
#include <SDL_image.h>
#include <SDL.h>



void initialize_paddle(Paddle* paddle, SDL_Renderer* renderer) {
    paddle->x = 400;
    paddle->y = 550;
    paddle->width = 120;
    paddle->height = 25;
    paddle->speed = 15;
    SDL_Surface* surface = IMG_Load("paddle.JPG");
    paddle->texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

void update_paddle(Paddle* paddle, float delta_time) {
    const Uint8* state = SDL_GetKeyboardState(NULL);

    if (state[SDL_SCANCODE_LEFT]) {
        //paddle->x -= paddle->speed * delta_time;
        if (paddle->x - paddle->width/2 > 0) { // check if paddle is at the left edge
            paddle->x -= paddle->speed * delta_time;
        }
    }
    if (state[SDL_SCANCODE_RIGHT]) {
        //paddle->x += paddle->speed * delta_time;
        if (paddle->x + paddle->width/2 < 800) { // check if paddle is at the right edge
            paddle->x += paddle->speed * delta_time;
        }
    }
}

void render_paddle(Paddle* paddle, SDL_Renderer* renderer) {
    SDL_Rect rect = {paddle->x - paddle->width/2, paddle->y - paddle->height/2, paddle->width, paddle->height};
    SDL_RenderCopy(renderer, paddle->texture, NULL, &rect);
}


void destroy_paddle(Paddle* paddle) {

    SDL_DestroyTexture(paddle->texture);
}







