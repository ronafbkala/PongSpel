
#include "counter.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "ball.h"

void increaseScore(int player, Score* score) {
    if (player == 1) {
        score->player1++;
    } else if (player == 2) {
        score->player2++;
    }
}


void drawScore(SDL_Renderer* renderer, TTF_Font* font, Score score) {
    char scoreText[64];
    sprintf(scoreText, "Player 1: %d - Player 2: %d", score.player1, score.player2);
    
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, scoreText, (SDL_Color) {255, 255, 255, 255});
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    SDL_Rect textRect = { 0, 0, textSurface->w, textSurface->h};
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

