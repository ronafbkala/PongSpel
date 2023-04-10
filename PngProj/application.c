#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include "application.h"
#include "ball.h"
#include "paddle.h"

void run_application()
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("My Pong Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    SDL_Event event;
    int quit = 0;

    Ball ball;
    initialize_ball(&ball, renderer);

    Paddle paddle; 
    initialize_paddle(&paddle, renderer);
    int play_button_pressed = 0;
    int game_over = 0;

    // Load the Play button image
    SDL_Surface* play_button_surface = IMG_Load("play.png");
    if (!play_button_surface) {
        SDL_Log("Failed to load image: %s", IMG_GetError());
        return;
    }
    SDL_Texture* play_button_texture = SDL_CreateTextureFromSurface(renderer, play_button_surface);
    SDL_FreeSurface(play_button_surface);
    SDL_Surface* game_over_surface = IMG_Load("game_over.jpg");
    if (!game_over_surface) {
        SDL_Log("Failed to load image: %s", IMG_GetError());
        return;
    }
    SDL_Texture* game_over_texture = SDL_CreateTextureFromSurface(renderer, game_over_surface);
    SDL_FreeSurface(game_over_surface);

    // Set the Play button position and size
    SDL_Rect play_button_rect = { 0, 0, 100, 50 };
    SDL_GetWindowSize(window, &play_button_rect.x, &play_button_rect.y);
    play_button_rect.x /= 2;
    play_button_rect.y /= 2;
    play_button_rect.x -= play_button_rect.w / 2;
    play_button_rect.y -= play_button_rect.h / 2;

    while (!quit)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                quit = 1;
            }
             else if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                int x = event.button.x;
                int y = event.button.y;

                // Check if the mouse click was inside the Play button
                if (x >= play_button_rect.x && x <= play_button_rect.x + play_button_rect.w &&
                    y >= play_button_rect.y && y <= play_button_rect.y + play_button_rect.h)
                {
                    play_button_pressed = 1;
                }
            }
    }

    SDL_RenderClear(renderer);
    if (!play_button_pressed)
    {
        // Draw the Play button image
        SDL_RenderCopy(renderer, play_button_texture, NULL, &play_button_rect);
    }
    else if(!game_over)
    {
    // Move the ball
    update_ball(&ball,&paddle, 0.016f);
     //update_ball(&ball, 0.016f);
     render_ball(&ball, renderer);

    // Move and render the paddle
     update_paddle(&paddle, 0.016f);
     render_paddle(&paddle, renderer);
     // check collision with bottom wall
    if(ball.y + ball.radius >= 600) {
        game_over = 1;
    }
     else{
    // draw the game over image
    SDL_Rect game_over_rect = {0, 0, 300, 100};
    SDL_GetWindowSize(window, &game_over_rect.w, &game_over_rect.h);
    game_over_rect.x = (game_over_rect.w - game_over_rect.w) / 2;
    game_over_rect.y = (game_over_rect.h - game_over_rect.h) / 2;

    SDL_RenderCopy(renderer, game_over_texture, NULL, &game_over_rect);
    }
        
        SDL_RenderPresent(renderer);
    }

    destroy_ball(&ball);
    destroy_paddle(&paddle); 
    SDL_DestroyTexture(play_button_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
