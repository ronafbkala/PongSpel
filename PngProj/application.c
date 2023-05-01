#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "application.h"
#include "ball.h"
#include "paddle.h"
#include <SDL2/SDL_mixer.h>
#include<stdbool.h>

const int FPS = 60;
const int frameDelay = 1000 / FPS;

Uint32 frameStart;
int frameTime;

void initialize_sdl() 
{
    SDL_Init(SDL_INIT_VIDEO);
}

void initialize_sdl_mixer()
{
    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) < 0)
    {
        SDL_Log("Failed to initialize SDL_mixer: %s", Mix_GetError());
        return;
    }
}

Mix_Music* load_music()
{
    Mix_Music* music = Mix_LoadMUS("Cyberpunk_Moonlight_Sonata.mp3");
    if (!music)
    {
        SDL_Log("Failed to load music: %s", Mix_GetError());
        return NULL;
    }
    return music;
}

int play_music(Mix_Music* music) 
{
    if (Mix_PlayMusic(music, -1) < 0)
    {
        SDL_Log("Failed to play music: %s", Mix_GetError());
        return 0;
    }
    return 1;
}

SDL_Window* create_window()
{
    return SDL_CreateWindow("My Pong Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
}

SDL_Renderer* create_renderer(SDL_Window* window)
{
    return SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void set_render_draw_color(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}
//////////////////////////
void initialize_game_objects(SDL_Renderer* renderer, Ball* ball, Paddle** paddles)
{
    initialize_ball(ball, renderer);
    paddles[0] = initialize_paddle(renderer);
    paddles[1] = initialize_paddle2(renderer);
    paddles[2] = initialize_paddle3(renderer);
    paddles[3] = initialize_paddle4(renderer);
}

SDL_Texture* load_texture(SDL_Renderer* renderer, const char* path)
{
    SDL_Surface* surface = IMG_Load(path);
    if (!surface) {
        SDL_Log("Failed to load image: %s", IMG_GetError());
        return NULL;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

void set_play_button_rect(SDL_Window* window, SDL_Rect* play_button_rect) 
{
    SDL_GetWindowSize(window, &play_button_rect->x, &play_button_rect->y);
    play_button_rect->x /= 2;
    play_button_rect->y /= 2;
    play_button_rect->x -= play_button_rect->w / 2;
    play_button_rect->y -= play_button_rect->h / 2;
}

void handle_events(SDL_Event event, int* quit, int* play_button_pressed, SDL_Rect play_button_rect) {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            *quit = 1;
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN) {
            int x = event.button.x;
            int y = event.button.y;

            // Check if the mouse click was inside the Play button
            if (x >= play_button_rect.x && x <= play_button_rect.x + play_button_rect.w &&
                y >= play_button_rect.y && y <= play_button_rect.y + play_button_rect.h)
            {
                *play_button_pressed = 1;
            }
        }
    }
}

void update_objects(Ball* ball, Paddle* paddles[], SDL_Renderer* renderer, float deltaTime) {
    // Update and render the ball
    update_ball(ball, paddles[0], paddles[1], paddles[2], paddles[3], deltaTime);
    render_ball(ball, renderer);
    
    // Update and render the paddles
    
    update_paddle(paddles[0], paddles[1], paddles[2], paddles[3], deltaTime);
    render_paddle(paddles[0], renderer);
    render_paddle(paddles[1], renderer);
    render_paddle(paddles[2], renderer);
    render_paddle(paddles[3], renderer);
    
}

bool check_collision(Ball* ball) {
    if (ball->y + ball->radius >= 600 || ball->y - ball->radius <= 0 || ball->x - ball->radius <= 0 || ball->x + ball->radius >= 800) {
        return 1;
    }
    return 0;
}







void run_application()
{
    initialize_sdl();
    initialize_sdl_mixer();

    Mix_Music* music = load_music();
    if (!music)
    {
        SDL_Log("Failed to load music");
    }
    if (!play_music(music))
    {
        SDL_Log("Failed to play music");
    }
    
    SDL_Window* window = create_window();
    SDL_Renderer* renderer = create_renderer(window);

    set_render_draw_color(renderer);

    Ball ball;
    Paddle* paddles[4];
    initialize_game_objects(renderer, &ball, paddles);

    SDL_Texture* play_button_texture = load_texture(renderer, "play.png");
    SDL_Texture* game_over_texture = load_texture(renderer, "game_over.jpg");
   
    
    // Set the Play button position and size
    SDL_Rect play_button_rect;
    SDL_QueryTexture(play_button_texture, NULL, NULL, &play_button_rect.w, &play_button_rect.h);
    set_play_button_rect(window, &play_button_rect);

    
    ///////////////////////////////////////-2-
    int quit = 0;
    int play_button_pressed=0;
    int game_over = 0;

    while (!quit)
    {
        frameStart = SDL_GetTicks();

        SDL_Event event;
        handle_events(event, &quit, &play_button_pressed, play_button_rect);
        SDL_RenderClear(renderer);

        if (!play_button_pressed) {
            // Draw the Play button image
            SDL_RenderCopy(renderer, play_button_texture, NULL, &play_button_rect);
        }
        else if (!game_over) {
            // Move,render the ball and the paddle
            update_objects(&ball, paddles, renderer, 0.018f);

            // check collision with all walls
            if (check_collision(&ball)) {
                game_over = 1;
                // Stoppar musiken
                Mix_HaltMusic();
                // Stänger ner ljudsystemet
                Mix_CloseAudio();
            }
        }
        else {
            SDL_Rect game_over_rect = { 0, 0, 300, 100 };
            SDL_GetWindowSize(window, &game_over_rect.w, &game_over_rect.h);
            game_over_rect.x = (game_over_rect.w - game_over_rect.w) / 2;
            game_over_rect.y = (game_over_rect.h - game_over_rect.h) / 2;

            SDL_RenderCopy(renderer, game_over_texture, NULL, &game_over_rect);
        }

        SDL_RenderPresent(renderer);

        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    // Free game objects
    destroy_ball(&ball);
    destroy_paddle(paddles[0], paddles[1], paddles[2], paddles[3]);

    // Free resources
    SDL_DestroyTexture(game_over_texture);
    SDL_DestroyTexture(play_button_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_FreeMusic(music);
    Mix_CloseAudio();
    SDL_Quit();
}
