#include "ball.h"
#include <SDL2/SDL_image.h>
#include <math.h>

#define PI 3.14159265f
int lastCollidedDown = 0;
int lastCollidedUp = 0;
int lastCollidedLeft = 0;
int lastCollidedRight = 0;

int lastCollidedDownWall = 0;
int lastCollidedUpWall = 0;
int lastCollidedLeftWall = 0;
int lastCollidedRightWall = 0;



void update_ball_speed(Ball* ball){
    if(ball->speed < 300.0f){
        ball->speed *= 1.1f;
    }
}

void initialize_ball(Ball* ball, SDL_Renderer* renderer) {
    ball->x = 400;
    ball->y = 300;
    ball->dx = cosf(45 * PI / 180);
    ball->dy = sinf(45 * PI / 180);
    ball->speed = 200;
    ball->radius = 25;
    SDL_Surface* surface = IMG_Load("ball.png");
    ball->texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

SDL_Renderer *renderer = NULL;


void update_ball(Ball* ball, Paddle* paddle, Paddle* paddle2, Paddle* paddle3, Paddle* paddle4, float delta_time) {
    ball->x += ball->dx * ball->speed * delta_time;
    ball->y += ball->dy * ball->speed * delta_time;

    // Create kollision detectors
    SDL_Rect ballRect = {
        .x = ball->x,
        .y = ball->y,
        .w = ball->radius,
        .h = ball->radius,
    };

    SDL_Rect playerDownRect = {
        .x = paddle->x,
        .y = paddle->y,
        .w = paddle->width,
        .h = paddle->height,
    };

    SDL_Rect playerRightRect = {
        .x = paddle2->x,
        .y = paddle2->y,
        .w = paddle2->width,
        .h = paddle2->height,
    };

    SDL_Rect playerUpRect = {
        .x = paddle3->x,
        .y = paddle3->y,
        .w = paddle3->width,
        .h = paddle3->height,
    };

    SDL_Rect playerLeftRect = {
        .x = paddle4->x,
        .y = paddle4->y,
        .w = paddle4->width,
        .h = paddle4->height,
    };
// check collision with paddles
    if(SDL_HasIntersection(&ballRect, &playerDownRect)){
        if(!lastCollidedDown){
            printf("Direction change down \n");
            
            ball->dy *= -1;
            lastCollidedDown++;
            lastCollidedDownWall++;
        }            
        lastCollidedLeft = 0;
        lastCollidedRight = 0;
        lastCollidedUp = 0;
        lastCollidedLeftWall = 0;
        lastCollidedRightWall = 0;
        lastCollidedUpWall = 0;
        
    }

    if(SDL_HasIntersection(&ballRect, &playerRightRect)){
        if(!lastCollidedRight){
            printf("Direction change right\n");
            ball->dx *= -1;
            lastCollidedRight++;
            lastCollidedRightWall++;
        }
        lastCollidedDown = 0;
        lastCollidedLeft = 0;
        lastCollidedUp = 0;
        lastCollidedDownWall = 0;
        lastCollidedLeftWall = 0;
        lastCollidedUpWall = 0;
        
    }

    if(SDL_HasIntersection(&ballRect, &playerUpRect)){
        if(!lastCollidedUp){
            printf("Direction change up\n");
            ball->dy *= -1;
            lastCollidedUp++;
            lastCollidedUpWall++;
        }
        lastCollidedDown = 0;
        lastCollidedLeft = 0;
        lastCollidedRight = 0;
        lastCollidedDownWall = 0;
        lastCollidedLeftWall = 0;
        lastCollidedRightWall = 0;
    }

    if(SDL_HasIntersection(&ballRect, &playerLeftRect)){
        if(!lastCollidedLeft){
           printf("Direction change left\n");
           ball->dx *= -1;
           lastCollidedLeft++;
           lastCollidedLeftWall++;
        }
        lastCollidedDown = 0;
        lastCollidedRight = 0;
        lastCollidedUp = 0;
        lastCollidedDownWall = 0;
        lastCollidedRightWall = 0;
        lastCollidedUpWall = 0;
    }

    // check collision with walls
    if (ball->x - ball->radius < 0) {
        if(!lastCollidedLeftWall){
            printf("Direction change wall left\n");
            ball->dx *= -1;
            lastCollidedLeftWall++;
            lastCollidedLeft++;
        }
        lastCollidedDownWall = 0;
        lastCollidedRightWall = 0;
        lastCollidedUpWall = 0;
        lastCollidedDown = 0;
        lastCollidedRight = 0;
        lastCollidedUp = 0;
    }

    if (ball->x + ball->radius > 800) {
        if(!lastCollidedRightWall){
            printf("Direction change wall right\n");
            ball->dx *= -1;
            lastCollidedRightWall++;
            lastCollidedRight++;
        }
        lastCollidedDownWall = 0;
        lastCollidedLeftWall = 0;
        lastCollidedUpWall = 0;
        lastCollidedDown = 0;
        lastCollidedLeft = 0;
        lastCollidedUp = 0;
    }
    if (ball->y - ball->radius < 0) {
        if(!lastCollidedUp){
            printf("Direction change wall up\n");
            ball->dy *= -1;
            lastCollidedUpWall++;
            lastCollidedUp++;
        }
        lastCollidedDownWall = 0;
        lastCollidedLeftWall = 0;
        lastCollidedRightWall = 0;
        lastCollidedDown = 0;
        lastCollidedLeft = 0;
        lastCollidedRight = 0;
    }
    if (ball->y + ball->radius > 600) {
        if(!lastCollidedDownWall){
            printf("Direction change wall down\n");
            ball->dy *= -1;
            lastCollidedDownWall++;
            lastCollidedDown++;
        }
        lastCollidedUpWall = 0;
        lastCollidedLeftWall = 0;
        lastCollidedRightWall = 0;
        lastCollidedUp = 0;
        lastCollidedLeft = 0;
        lastCollidedRight = 0;
    }
}


void render_ball(Ball* ball, SDL_Renderer* renderer) {
    SDL_Rect rect = {ball->x - ball->radius, ball->y - ball->radius, ball->radius * 2, ball->radius * 2};
    SDL_RenderCopy(renderer, ball->texture, NULL, &rect);
}
void destroy_ball(Ball* ball) {
    SDL_DestroyTexture(ball->texture);
}
