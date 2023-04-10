#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include "application.h"
#include "ball.h"

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



    while (!quit)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                quit = 1;
            }
        }

        SDL_RenderClear(renderer);

        
        // Move the ball
        update_ball(&ball, 0.016f);
        //update_ball(&ball, 0.016f);
        render_ball(&ball, renderer);

        SDL_RenderPresent(renderer);
    }

    destroy_ball(&ball);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
