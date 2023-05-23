
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "application.h"
#include "ball.h"
#include "paddle.h"
#include "collision.h"
#include "counter.h"
#include <SDL2/SDL_mixer.h>
#include <stdbool.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_ttf.h>
#define SCORE_TEXT_SIZE 20

int point_gameOver = 0;


int check_paddle_life(Paddle *paddle, int *live_chance) {              // Destroy paddle om score för en paddle lika med 0 
    (*live_chance)--;
    if (*live_chance == point_gameOver) {
        SDL_DestroyTexture(paddle->texture);                          
        free(paddle); 
        return 1;
    }
    return 0;
}




void initializeScore(Player *all_players_info)      // Set alla plyers score to three as a score int/text i början/
 {                   
for (int i = 0; i < 4; i++) {            
   all_players_info[i].score = 3;  
}
}

 
SDL_Rect scoreRect[] = {
    { 350, 480, 80, 60 },  // Player 0
    { 650, 200, 80, 60 },  // Player 1
    { 350, 70, 80, 60 },   // Player 2
    { 80, 200, 80, 60 }    // Player 3
};


 // To draw the score on the top of the screen for all players, put it as a string text i case adding some texts later 
/*void drawScore(Player* all_players_info, TTF_Font* font, SDL_Renderer* renderer, SDL_Window* window) {
    const int numPlayers = 4;
    char scoreText[numPlayers][SCORE_TEXT_SIZE];
    SDL_Surface* textSurface[numPlayers];
    SDL_Texture* textTexture[numPlayers];

    for (int i = 0; i < numPlayers; i++) {
        if (all_players_info[i].score >= 0) {
            snprintf(scoreText[i], sizeof(scoreText[i]), "%d", all_players_info[i].score);
        } else {
            snprintf(scoreText[i], sizeof(scoreText[i]), "0");
        }

        textSurface[i] = TTF_RenderText_Solid(font, scoreText[i], (SDL_Color){ 255, 255, 255, 255 });
        textTexture[i] = SDL_CreateTextureFromSurface(renderer, textSurface[i]);

        SDL_RenderCopy(renderer, textTexture[i], NULL, &scoreRect[i]);
    }

    for (int i = 0; i < numPlayers; i++) {
        SDL_FreeSurface(textSurface[i]);
        SDL_DestroyTexture(textTexture[i]);
    }
}*/
void drawScore(Player* all_players_info, TTF_Font* font, SDL_Renderer* renderer, SDL_Window* window) {
    const int numPlayers = 4;
    char scoreText[numPlayers][SCORE_TEXT_SIZE];
    SDL_Surface* textSurface[numPlayers];
    SDL_Texture* textTexture[numPlayers];

    for (int i = 0; i < numPlayers; i++) {
        if (all_players_info[i].score > 0) {
            snprintf(scoreText[i], sizeof(scoreText[i]), "%d", all_players_info[i].score);
        } else {
            snprintf(scoreText[i], sizeof(scoreText[i]), "Player %d loses", i + 1);
        }

        textSurface[i] = TTF_RenderText_Solid(font, scoreText[i], (SDL_Color){ 255, 255, 255, 255 });
        textTexture[i] = SDL_CreateTextureFromSurface(renderer, textSurface[i]);

        SDL_RenderCopy(renderer, textTexture[i], NULL, &scoreRect[i]);
    }

    for (int i = 0; i < numPlayers; i++) {
        SDL_FreeSurface(textSurface[i]);
        SDL_DestroyTexture(textTexture[i]);
    }
}
bool checkWinner(Player* all_players_info, int numPlayers, int* winnerIndex) {
    int count = 0;
    
    for (int i = 0; i < numPlayers; i++) {
        if (all_players_info[i].score > 0) {
            count++;
            *winnerIndex = i;
        }
    }
    
    if (count == 1) {
        printf("Player %d is the winner!\n", *winnerIndex + 1);
        return true;
    }
    
    return false;
}


void endGame(SDL_Renderer* renderer, TTF_Font* font, SDL_Window* window, int *winnerIndex, SDL_Texture *win_texture) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    char winnerText[50];
    snprintf(winnerText, sizeof(winnerText), "Player %d is the winner!", *winnerIndex + 1);
    
    SDL_Surface* messageSurface = TTF_RenderText_Solid(font, winnerText, (SDL_Color){ 255, 255, 255, 255 });
    SDL_Texture* messageTexture = SDL_CreateTextureFromSurface(renderer, messageSurface);
    
    int messageWidth = messageSurface->w;
    int messageHeight = messageSurface->h;
    SDL_Rect messageRect = { (800 - messageWidth) / 2, (600 - messageHeight) / 2, messageWidth, messageHeight };

     SDL_RenderCopy(renderer, win_texture, NULL, NULL); 
    
    SDL_RenderCopy(renderer, messageTexture, NULL, &messageRect);
    
    SDL_FreeSurface(messageSurface);
    SDL_DestroyTexture(messageTexture);
    
    SDL_RenderPresent(renderer);
    
    SDL_Delay(10000); // 10 seconds
    
    SDL_DestroyWindow(window);
    SDL_Quit();
}




