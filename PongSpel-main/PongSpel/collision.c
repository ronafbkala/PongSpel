#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "application.h"
# include "collision.h"
#include "ball.h"
#include "paddle.h"
#include <stdbool.h>
#define MAX_PLAYER_POINTS 3


int live_chance_paddle = 0;
int live_chance_paddle2 = 0;
int live_chance_paddle3 =0; 
int live_chance_paddle4 = 0; 

int point_gameOver = 100000000;


void initPlayers(Player *all_players_info) {                   // Set alla plyers score zero i början
for (int i = 0; i < 4; i++) {            
   all_players_info[i].score =0;  
}
}

int check_paddle_life(Paddle *paddle, int *live_chance) {              // Destroy paddle om antal liv för en paddle lika med 3 
    (*live_chance)++;
    if (*live_chance == point_gameOver) {
        SDL_DestroyTexture(paddle->texture);
        free(paddle);
        return 1;
    }
    return 0;
}

bool check_collision(Ball *ball, Paddle *paddle, Paddle *paddle2, Paddle *paddle3, Paddle *paddle4, Player* all_players_info, SDL_Renderer *renderer, TTF_Font *font, SDL_Window *window) {
    if (ball->y + ball->radius >= 600) {                                // Check if the ball hit the bottom
       all_players_info[0].score ++;                                      // Increase player's struct info to player  1
        if(all_players_info[0].score<=MAX_PLAYER_POINTS) {
          printScore(renderer, font, all_players_info,window);          // print method
        }
        return check_paddle_life(paddle, &live_chance_paddle);
    }
    
    if (ball->y - ball->radius <= 0) {                             // Check if the ball hit the top side
       all_players_info[2].score ++;                                // // Increase player's struct info to player  3
       if( all_players_info[2].score <= MAX_PLAYER_POINTS) {
       printScore(renderer, font, all_players_info,window);              
       }
        return check_paddle_life(paddle3, &live_chance_paddle3);
    }

    if (ball->x - ball->radius <= 0) {                           // Check if the ball hit the left side 
         all_players_info[3].score++;                                  // // Increase player's struct info to player  4
        if(all_players_info[3].score<=MAX_PLAYER_POINTS) {
        printScore(renderer, font, all_players_info,window);             
        }
        return check_paddle_life(paddle4, &live_chance_paddle4);      
    }

    if (ball->x + ball->radius >= 800) {                          // Check if the ball hit the right side 
          all_players_info[1].score++;                                  // Increase player's struct info to player  2
             if(all_players_info[1].score<=MAX_PLAYER_POINTS) {
          printScore(renderer, font, all_players_info,window);          
        }                  
        return check_paddle_life(paddle2, &live_chance_paddle2); 
    }
    return 0;
}


// To draw the score on the top of the screen for all players
void printScore(SDL_Renderer *renderer, TTF_Font *font,  Player* all_player_info, SDL_Window *window)
{
    char scoreText[100];
    sprintf(scoreText, "Player 1: Score %d   |   Player 2: Score %d   |   player 3: Score %d   |   player 4: Score %d", 
                        all_player_info[0].score, all_player_info[1].score, all_player_info[2].score, all_player_info[3].score);
   
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, scoreText, (SDL_Color) {255, 255, 255, 255});
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

   SDL_SetWindowTitle(window, scoreText);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);

}




