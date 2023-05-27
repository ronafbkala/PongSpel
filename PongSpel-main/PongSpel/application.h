#ifndef APPLICATION_H
#define APPLICATION_H
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

struct data 
{
	int playerIndex;
	float downPaddle_x, downPaddle_y;
	float upPaddle_x, upPaddle_y;
	float rightPaddle_x, rightPaddle_y;
	float leftPaddle_x, leftPaddle_y;
	float ball_x, ball_y;
};
typedef struct data Data;


void initialize_sdl();
void initialize_sdl_mixer();
Mix_Music *load_music(char *sound);
void play_music(Mix_Music *music);
Mix_Chunk* loadSoundEffect();
SDL_Window *create_window();
SDL_Renderer *create_renderer(SDL_Window *window);
void initialize_game_objects(SDL_Renderer *renderer, Ball *ball, Paddle **paddles);
SDL_Texture *load_texture(SDL_Renderer *renderer, const char *path);
int is_point_in_rect(int x, int y, SDL_Rect rect);
void handle_events(SDL_Event event, int *quit, int *playButton, int *closeButton, int *joinButton, int *infoButton,  SDL_Rect playRect, SDL_Rect closeRect, SDL_Rect  joinRect, SDL_Rect infoRect, int *infoShown);
void update_objects(Ball *ball, Paddle *paddles[], SDL_Renderer *renderer, float deltaTime, int playerIndex, Data *gameData);
void initializeSDLTTF();
void moveAllPaddles (Paddle* paddles[], Data *gameData, int myPlayerIndex);
void update_background(SDL_Renderer *renderer, SDL_Texture *background_texture, int *background_offset);
void initializeSDLNet(UDPsocket* sd, IPaddress* srvadd, UDPpacket** p, UDPpacket** pRecieve);
void cleanupResources(SDL_Texture* play_button_texture, SDL_Texture* background_texture,SDL_Texture* win_texture, SDL_Texture* background_game_texture, SDL_Renderer* renderer,
    SDL_Window* window, SDL_Texture* instruction_texture, Mix_Music* music3,Mix_Chunk* SoundEffect);

#endif /* APPLICATION_H */