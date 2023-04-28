#include "paddle.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <stdlib.h>


Paddle* initialize_paddle(SDL_Renderer* renderer) {
    Paddle* paddle = malloc(sizeof(Paddle));
    if (!paddle) {
        fprintf(stderr, "Failed to allocate memory for paddle\n");
        return NULL;
    }
    paddle->x = 400;
    paddle->y = 590;
    paddle->width = 120;
    paddle->height = 25;
    paddle->speed = 300;
    SDL_Surface* surface = IMG_Load("paddle.JPG");
    paddle->texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return paddle;
}
Paddle* initialize_paddle2(SDL_Renderer* renderer) {
    Paddle* paddle = malloc(sizeof(Paddle));
    if (!paddle) {
        fprintf(stderr, "Failed to allocate memory for paddle\n");
        return NULL;
    }
    paddle->x = 790;
    paddle->y = 200;
    paddle->width = 25;
    paddle->height = 120;
    paddle->speed = 300;

    SDL_Surface* surface = IMG_Load("paddle.JPG");
    paddle->texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return paddle;
}

Paddle* initialize_paddle3(SDL_Renderer* renderer) {
    Paddle* paddle = malloc(sizeof(Paddle));
    if (!paddle) {
        fprintf(stderr, "Failed to allocate memory for paddle\n");
        return NULL;
    }
    paddle->x = 400;
    paddle->y = 10;
    paddle->width = 120;
    paddle->height = 25;
    paddle->speed = 300;
    SDL_Surface* surface = IMG_Load("paddle.JPG");
    paddle->texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return paddle;
}

Paddle* initialize_paddle4(SDL_Renderer* renderer) {
    Paddle* paddle = malloc(sizeof(Paddle));
    if (!paddle) {
        fprintf(stderr, "Failed to allocate memory for paddle\n");
        return NULL;
    }
    paddle->x = 10;
    paddle->y = 200;
    paddle->width = 25;
    paddle->height = 120;
    paddle->speed = 300;
    

    SDL_Surface* surface = IMG_Load("paddle.JPG");
    paddle->texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return paddle;
}


void update_paddle(Paddle* paddle, Paddle* paddle2, Paddle* paddle3, Paddle* paddle4, float delta_time) {
    const Uint8* state = SDL_GetKeyboardState(NULL);

    // update paddle 1
    if (state[SDL_SCANCODE_LEFT]) {
        if (paddle->x - paddle->width/2 > 0) { // check if paddle is at the left edge
            paddle->x -= paddle->speed * delta_time;
        }
    }
    if (state[SDL_SCANCODE_RIGHT]) {
        if (paddle->x + paddle->width/2 < 800) { // check if paddle is at the right edge
            paddle->x += paddle->speed * delta_time;
        }
    }

    // update paddle 2
    if (state[SDL_SCANCODE_UP]) {
        if (paddle2->y - paddle2->height/2 > 0) { // check if paddle is at the top edge
            paddle2->y -= paddle2->speed * delta_time;
        }
    }
    if (state[SDL_SCANCODE_DOWN]) {
        if (paddle2->y + paddle2->height/2 < 600) { // check if paddle is at the bottom edge
            paddle2->y += paddle2->speed * delta_time;
        }
    }

    // update paddle 3
    if (state[SDL_SCANCODE_A]) {
        if (paddle3->x - paddle3->width/2 > 0) { // check if paddle is at the left edge
            paddle3->x -= paddle3->speed * delta_time;
        }
    }
    if (state[SDL_SCANCODE_D]) {
        if (paddle3->x + paddle3->width/2 < 800) { // check if paddle is at the right edge
            paddle3->x += paddle3->speed * delta_time;
        }
    }

    // update paddle 4
    if (state[SDL_SCANCODE_W]) {
        if (paddle4->y - paddle4->height/2 > 0) { // check if paddle is at the top edge
            paddle4->y -= paddle4->speed * delta_time;
        }
    }
    if (state[SDL_SCANCODE_S]) {
        if (paddle4->y + paddle4->height/2 < 600) { // check if paddle is at the bottom edge
            paddle4->y += paddle4->speed * delta_time;
        }
    }
}



void render_paddle(Paddle* paddle, SDL_Renderer* renderer) {
    SDL_Rect rect = {paddle->x - paddle->width/2, paddle->y - paddle->height/2, paddle->width, paddle->height};
    SDL_RenderCopy(renderer, paddle->texture, NULL, &rect);
}


void destroy_paddle(Paddle* paddle, Paddle* paddle2, Paddle* paddle3, Paddle* paddle4) {

    SDL_DestroyTexture(paddle->texture);
    SDL_DestroyTexture(paddle2->texture);
    SDL_DestroyTexture(paddle3->texture);
    SDL_DestroyTexture(paddle4->texture);
    free(paddle);
    free(paddle2);
    free(paddle3);
    free(paddle4);
}







