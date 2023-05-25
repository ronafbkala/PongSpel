#include "paddle.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <stdlib.h>


Paddle* initialize_paddle(SDL_Renderer* renderer, int x, int y, int width, int height, int index) {
    Paddle* paddle = malloc(sizeof(Paddle));
    if (!paddle) {
        fprintf(stderr, "Failed to allocate memory for paddle\n");
        return NULL;
    }
    paddle->x = x;
    paddle->y = y;
    paddle->width = width;
    paddle->height = height;
    paddle->index = index;
    paddle->speed = 300;
    SDL_Surface* surface = IMG_Load("src/paddle.png");
    paddle->texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return paddle;
}


void update_paddle(Paddle* paddle, float delta_time, int playerIndex)
{
    const Uint8* state = SDL_GetKeyboardState(NULL);

    if ((state[SDL_SCANCODE_A] && playerIndex == 1) || (state[SDL_SCANCODE_A] && playerIndex == 3))
    {
        if (paddle->x - paddle->width / 2 > 0) // check if paddle is at the left edge
        {
            paddle->x -= paddle->speed * delta_time;
        }
    }
    if ((state[SDL_SCANCODE_D] && playerIndex == 1) || (state[SDL_SCANCODE_D] && playerIndex == 3))
    {
        if (paddle->x + paddle->width / 2 < 800) // check if paddle is at the right edge
        {
            paddle->x += paddle->speed * delta_time;
        }
    }
    if ((state[SDL_SCANCODE_W] && playerIndex == 2) || (state[SDL_SCANCODE_W] && playerIndex == 4))
    {
        if (paddle->y - paddle->height / 2 > 0) // check if paddle is at the top edge
        {
            paddle->y -= paddle->speed * delta_time;
        }
    }
    if ((state[SDL_SCANCODE_S] && playerIndex == 2) || (state[SDL_SCANCODE_S] && playerIndex == 4))
    {
        if (paddle->y + paddle->height / 2 < 600) // check if paddle is at the bottom edge
        {
            paddle->y += paddle->speed * delta_time;
        }
    }
}



void render_paddle(Paddle* paddle, SDL_Renderer* renderer) {
    SDL_Rect rect = {paddle->x - paddle->width/2, paddle->y - paddle->height/2, paddle->width, paddle->height};
    SDL_RenderCopy(renderer, paddle->texture, NULL, &rect);
}


void destroy_paddle(Paddle* paddle) {

    SDL_DestroyTexture(paddle->texture);
    free(paddle);
    
}







