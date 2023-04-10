#include "ball.h"
#include <SDL_image.h>
#include <math.h>

#define PI 3.14159265f

void initialize_ball(Ball* ball, SDL_Renderer* renderer) {
    ball->x = 400;
    ball->y = 300;
    ball->dx = cosf(45 * PI / 180);
    ball->dy = sinf(45 * PI / 180);
    ball->speed = 15;
    ball->radius = 25;
    SDL_Surface* surface = IMG_Load("ball.png");
    ball->texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

void update_ball(Ball* ball, float delta_time) {
    ball->x += ball->dx * ball->speed * delta_time;
    ball->y += ball->dy * ball->speed * delta_time;
    if (ball->x - ball->radius < 0 || ball->x + ball->radius > 800) {
        ball->dx *= -1;
    }
    if (ball->y - ball->radius < 0 || ball->y + ball->radius > 600) {
        ball->dy *= -1;
    }
}

void render_ball(Ball* ball, SDL_Renderer* renderer) {
    SDL_Rect rect = {ball->x - ball->radius, ball->y - ball->radius, ball->radius * 2, ball->radius * 2};
    SDL_RenderCopy(renderer, ball->texture, NULL, &rect);
}
void destroy_ball(Ball* ball) {
    SDL_DestroyTexture(ball->texture);
}