#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "application.h"
#include "collision.h"
#include  "counter.h"
#include "ball.h"
#include "paddle.h"
#include <stdbool.h>
#define MAXIMUM_PLAYER_POINTS 3
#define MINIMUM_PLAYER_POINTS 0

int live_chance_paddle = MAXIMUM_PLAYER_POINTS;
int live_chance_paddle2 = MAXIMUM_PLAYER_POINTS;
int live_chance_paddle3 = MAXIMUM_PLAYER_POINTS;
int live_chance_paddle4 = MAXIMUM_PLAYER_POINTS;


bool check_collision(Ball* ball, Paddle* paddle, Paddle* paddle2, Paddle* paddle3, Paddle* paddle4, Player* all_players_info, SDL_Renderer* renderer, TTF_Font* font, SDL_Window* window)
{
    if (ball->y + ball->radius >= 600) // Check if the ball hit the bottom
    {
        all_players_info[0].score--; // Decrease player's array struct info score for player 1
        
         drawScore(all_players_info, font, renderer,window);  // Update the window title with hearts

        return check_paddle_life(paddle, &live_chance_paddle);
    }

    if (ball->y - ball->radius <= 0) // Check if the ball hit the top side
    {
        all_players_info[2].score--; // Decrease player's array struct info score for player 3
         drawScore(all_players_info, font, renderer,window);  // Update the window title with hearts

        return check_paddle_life(paddle3, &live_chance_paddle3);
    }

    if (ball->x - ball->radius <= 0) // Check if the ball hit the left side
    {
        all_players_info[3].score--; // Decrease player's array struct info score for player 4
         drawScore(all_players_info, font, renderer,window);  // Update the window title with hearts

        return check_paddle_life(paddle4, &live_chance_paddle4);
    }

    if (ball->x + ball->radius >= 800) // Check if the ball hit the right side
    {
        all_players_info[1].score--; // Decrease player's array struct info score for player 2
         drawScore(all_players_info, font, renderer,window);  // Update the window title with hearts

        return check_paddle_life(paddle2, &live_chance_paddle2);
    }
    return false;
}


   








