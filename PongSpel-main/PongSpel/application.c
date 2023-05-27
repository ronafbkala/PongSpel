#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "application.h"
#include "ball.h"
#include "paddle.h"
#include "collision.h"
#include <SDL2/SDL_mixer.h>
#include <stdbool.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_ttf.h>


void initialize_sdl()
{
    SDL_Init(SDL_INIT_VIDEO);
}

void initialize_sdl_mixer()
{
    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) < 0)
    {
        SDL_Log("Failed to initialize SDL_mixer: %s", Mix_GetError());
        
    }
}

Mix_Music *load_music(char *sound)
{
    Mix_Music *music = Mix_LoadMUS(sound);
    if (!music)
    {
        SDL_Log("Failed to load music: %s", Mix_GetError());
        return NULL;
    }
    return music;
}

void play_music(Mix_Music *music)
{
    if (Mix_PlayMusic(music, -1) < 0)
    {
        SDL_Log("Failed to play music: %s", Mix_GetError());
        
    }
}

Mix_Chunk* loadSoundEffect(){

Mix_Chunk *SoundEffect = Mix_LoadWAV("src/missball.mp3");
 if(!SoundEffect)
 {
    SDL_Log("Faild to load sound effect : '%s' ", Mix_GetError());
    return NULL; 
 } 

 return SoundEffect; 
}

SDL_Window *create_window()
{
    return SDL_CreateWindow("My Pong Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
}

SDL_Renderer *create_renderer(SDL_Window *window)
{
    return SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}


////////////////////////// vi ligger alla obj (4 paddle med olika positioner och ball i skärmen)
void initialize_game_objects(SDL_Renderer *renderer, Ball *ball, Paddle **paddles)
{
    initialize_ball(ball, renderer);
    paddles[0] = initialize_paddle(renderer, 400, 590, 120, 25, 1);
    paddles[1] = initialize_paddle(renderer, 790, 200, 25, 120, 2);
    paddles[2] = initialize_paddle(renderer, 400, 10, 120, 25, 3);
    paddles[3] = initialize_paddle(renderer, 10, 200, 25, 120, 4);
}

// för ladda alla objs bilder i skärmen
SDL_Texture *load_texture(SDL_Renderer *renderer, const char *path)
{
    SDL_Surface *surface = IMG_Load(path);
    if (!surface)
    {
        SDL_Log("Failed to load image: %s", IMG_GetError());
        return NULL;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

int is_point_in_rect(int x, int y, SDL_Rect rect)
{
    return (x >= rect.x && x <= rect.x + rect.w &&
            y >= rect.y && y <= rect.y + rect.h);
}

void handle_events(SDL_Event event, int *quit, int *playButton, int *closeButton, int *joinButton, int *infoButton,  SDL_Rect playRect, SDL_Rect closeRect, SDL_Rect  joinRect, SDL_Rect infoRect, int *infoShown)
{
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            *quit = 1;
            *closeButton=1;
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN)
        {
            int x = event.button.x;
            int y = event.button.y;

            // Check if the mouse click was inside the Play button
            if (is_point_in_rect(x, y, playRect))
            {
                *playButton = 1;
            }

            // Check if the mouse click was inside the Close button
            if (is_point_in_rect(x, y, closeRect))
            {
                *closeButton = 1;
            }

            // Check if the mouse click was inside the Join button
            if (is_point_in_rect(x, y, joinRect))
            {
                *joinButton = 1;
                //*playButton = 0;
                
            }

            // Check if the mouse click was inside the Info button
            if (is_point_in_rect(x, y, infoRect))
            {
                *infoButton = 1;
                *infoShown = 1; // Set infoShown to 1 to indicate that the instruction image should be shown
                         
            } 
        }
        else if (event.type == SDL_KEYDOWN)
        {
            if (event.key.keysym.sym == SDLK_RETURN) // Check if the pressed key is the Enter key
            {
                *infoShown = 0; // Set infoShown to 0 to return to the previous screen
            }
        }
        
    }
    
}

void update_objects(Ball *ball, Paddle *paddles[], SDL_Renderer *renderer, float deltaTime, int playerIndex, Data *gameData)
{
    // Update and render the ball
    //update_ball(ball, paddles[0], paddles[1], paddles[2], paddles[3], deltaTime);
    render_ball(ball, renderer);
    // Update and render the paddles
    render_paddle(paddles[0], renderer);
    render_paddle(paddles[1], renderer);
    render_paddle(paddles[2], renderer);
    render_paddle(paddles[3], renderer);
}

void initializeSDLTTF(){
    if (TTF_Init() != 0) { //använder SDL_ttf-biblioteket för att initiera ett TrueType-teckensnitt och ladda det från en teckensnittsfil på disken.
        printf("TTF_Init() failed: %s\n", SDL_GetError());
        
    }
}

void moveAllPaddles (Paddle* paddles[], Data *gameData, int myPlayerIndex)
{
    //printf("Flag 3 ----------- \n");
    if(myPlayerIndex != 1)
    {
        paddles[0]->x = gameData->downPaddle_x;
        paddles[0]->y = gameData->downPaddle_y;
    }
    if(myPlayerIndex != 2)
     {
        paddles[1]->x = gameData->rightPaddle_x;
        paddles[1]->y = gameData->rightPaddle_y;
     }
    if(myPlayerIndex != 3)
     {
        paddles[2]->x = gameData->upPaddle_x;
        paddles[2]->y = gameData->upPaddle_y;
     }
    if(myPlayerIndex != 4)
    {
        paddles[3]->x = gameData->leftPaddle_x;
        paddles[3]->y = gameData->leftPaddle_y;
    }

}
void update_background(SDL_Renderer *renderer, SDL_Texture *background_texture, int *background_offset)
{
    // Update the background offset
    *background_offset += 1;

    // Get the width and height of the background image
    int background_width, background_height;
    SDL_QueryTexture(background_texture, NULL, NULL, &background_width, &background_height);

    // Wrap the background offset around if it exceeds the width of the background image
    if (*background_offset > background_width)
        *background_offset = *background_offset % background_width;

    // Render the background
    SDL_Rect source_rect = { *background_offset, 0, background_width - *background_offset, background_height };
    SDL_Rect dest_rect = { 0, 0, background_width - *background_offset, background_height };
    SDL_RenderCopy(renderer, background_texture, &source_rect, &dest_rect);

    // If there is any remaining space, render the rest of the background
    if (background_width - *background_offset < background_width)
    {
        source_rect.x = 0;
        source_rect.y = 0;
        source_rect.w = *background_offset;
        source_rect.h = background_height;

        dest_rect.x = background_width - *background_offset;
        dest_rect.y = 0;
        dest_rect.w = *background_offset;
        dest_rect.h = background_height;

        SDL_RenderCopy(renderer, background_texture, &source_rect, &dest_rect);
    }
}


void initializeSDLNet(UDPsocket* sd, IPaddress* srvadd, UDPpacket** p, UDPpacket** pRecieve)
{
	if (SDLNet_Init() < 0)
	{
		fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	if (!(*sd = SDLNet_UDP_Open(0)))         // open a UDP socket
	{
		fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
		printf("called from main");
		exit(EXIT_FAILURE);
	}

	if (SDLNet_ResolveHost(srvadd, "127.0.0.1", 2000) == -1)
	{
		fprintf(stderr, "SDLNet_ResolveHost(192.0.0.1 2000): %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	if (!((*p = SDLNet_AllocPacket(512)) && (*pRecieve = SDLNet_AllocPacket(512))))    // two packets are allocated, one for sending data and one for receiving data, packet size 512
	{
		fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
}

void cleanupResources(SDL_Texture* play_button_texture, SDL_Texture* background_texture,
    SDL_Texture* win_texture, SDL_Texture* background_game_texture, SDL_Renderer* renderer,
    SDL_Window* window, SDL_Texture* instruction_texture, Mix_Music* music3,
    Mix_Chunk* SoundEffect)
{
    SDL_DestroyTexture(play_button_texture);
    SDL_DestroyTexture(background_texture);
    SDL_DestroyTexture(win_texture);
    SDL_DestroyTexture(background_game_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_DestroyTexture(instruction_texture);
    Mix_FreeMusic(music3);
    Mix_FreeChunk(SoundEffect);
    Mix_CloseAudio();
    SDL_Quit();
}