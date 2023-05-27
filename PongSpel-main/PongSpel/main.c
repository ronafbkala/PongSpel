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

const int FPS = 60;
const int frameDelay = 1000 / FPS;
Uint32 frameStart;
int frameTime;

int main(int argc, char* argv[])
{
    int quit = 0;
    int play_pressed = 0;
    int close_pressed = 0;
    int join_pressed = 0;
    int info_pressed = 0;
    int infoShown = 0;
    int joinTextShown = 0;
    int track = 0;
    int background_x =0;
    UDPsocket sd;
    IPaddress srvadd;
    srvadd.port = 2000;
    UDPpacket *p;
    UDPpacket *pRecive;
    int myPlayerIndex;
    int state = 0; // 0 is start 1 is ongoing
    Data gameData = {0};
    const int numPlayers = 4;
 
    float x_newPos;
    float y_newPos;
    initialize_sdl();
    initialize_sdl_mixer();
    initializeSDLNet(&sd, &srvadd, &p, &pRecive);

    Mix_Music *music1 = load_music("src/waitingSong.mp3");
    Mix_Music *music2 = load_music("src/backgroundSong.mp3");
    Mix_Music *music3 = load_music("src/win.mp3");
    Mix_Chunk *SoundEffect = loadSoundEffect();

    SDL_Window *window = create_window();
    SDL_Renderer *renderer = create_renderer(window);
    Ball ball;
    Paddle *paddles[4];
    initialize_game_objects(renderer, &ball, paddles);
    int isPlayerAlive[4] = {1,1,1,1};
	
    SDL_Texture *background_texture = load_texture(renderer, "src/moving-background.png");
    SDL_Texture *play_button_texture = load_texture(renderer, "src/play.png");
    SDL_Texture *exit_texture = load_texture(renderer, "src/exit.png");
    SDL_Texture *join_texture = load_texture(renderer, "src/join.png");
    SDL_Texture *info_texture = load_texture(renderer, "src/info.png");
    SDL_Texture *instruction_texture = load_texture(renderer, "src/instruction1.png");
    SDL_Texture *background_game_texture = load_texture(renderer, "src/background.jpg");
    SDL_Texture *win_texture = load_texture(renderer, "src/win.png");

    // Set button position and size
    SDL_Rect play_rect ={310, 265, 180, 70 };
    SDL_Rect close_rect ={310, 350, 180, 70 };
    SDL_Rect join_rect ={310, 50, 180, 70 };
    SDL_Rect info_rect ={310, 150, 180, 70 };
    SDL_Color textColor = { 255, 215, 0, 255 };

    initializeSDLTTF();
    TTF_Font* font = TTF_OpenFont("C:\\Windows\\Fonts\\arial.ttf", 50);  // för att ladda ett TrueType-teckensnitt från en teckensnittsfil på disken.
    if (!font) {
        SDL_Log("Failed to load font: %s", TTF_GetError());
    }
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, "Waiting for all clients", textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);    
    SDL_FreeSurface(textSurface);
    SDL_Rect textRect = { 800 / 2 - textSurface->w / 2, 600 / 2 - textSurface->h / 2, textSurface->w, textSurface->h };

    SDL_Surface* textSurface1 = TTF_RenderText_Solid(font, " ", textColor);
    
    Player all_players_info[4];                // struct för att lagra alla spelarens info  
    initializeScore(all_players_info);           // To Set alla plyers score to zero frön början
    while (!quit && !close_pressed) 
    {
        frameStart = SDL_GetTicks();
        SDL_Event event;
	    handle_events(event, &quit, &play_pressed, &close_pressed, &join_pressed, &info_pressed, play_rect, close_rect, join_rect, info_rect, &infoShown);
        SDL_RenderClear(renderer);
        switch (state){
            case 0:
                if (!play_pressed){
	                update_background(renderer, background_texture, &background_x);
                    SDL_RenderCopy(renderer, play_button_texture, NULL, &play_rect);

                }
		        else{
	                close_pressed = 0;

                    SDL_DestroyTexture(exit_texture);
                    SDL_DestroyTexture(play_button_texture);

                    update_background(renderer, background_texture, &background_x);
                    SDL_RenderCopy(renderer, join_texture, NULL, &join_rect);
                    SDL_RenderCopy(renderer, info_texture, NULL, &info_rect);
		        }
                if (infoShown)
		        {  
                    close_pressed=0;
                    SDL_RenderCopy(renderer, instruction_texture, NULL, NULL);  
                }
                if(join_pressed){// Render the text every frame as long as joinTextShown is set to 1
                    info_pressed=0;
                    infoShown=0;
                    close_pressed=0;
                    SDL_DestroyTexture(background_texture);
                    SDL_DestroyTexture(join_texture);
                    SDL_DestroyTexture(info_texture);
                    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
                    if(myPlayerIndex == 1){
                        textSurface1 = TTF_RenderText_Solid(font, "Down Paddle", textColor);
                        SDL_Texture* textTexture1 = SDL_CreateTextureFromSurface(renderer, textSurface1);    
                        SDL_FreeSurface(textSurface1);
                        SDL_Rect textRect1 ={310, 350, 180, 70 };
                        SDL_RenderCopy(renderer, textTexture1, NULL, &textRect1);
                    }
                    else if(myPlayerIndex == 2){
                        textSurface1 = TTF_RenderText_Solid(font, "Right Paddle", textColor);
                        SDL_Texture* textTexture1 = SDL_CreateTextureFromSurface(renderer, textSurface1);    
                        SDL_FreeSurface(textSurface1);
                        SDL_Rect textRect1 ={310, 350, 180, 70 };
                        SDL_RenderCopy(renderer, textTexture1, NULL, &textRect1);
                    }
                    else if(myPlayerIndex == 3){
                        
                        textSurface1 = TTF_RenderText_Solid(font, "Up Paddle", textColor);
                        SDL_Texture* textTexture1 = SDL_CreateTextureFromSurface(renderer, textSurface1);    
                        SDL_FreeSurface(textSurface1);
                        SDL_Rect textRect1 ={310, 350, 180, 70 };
                        SDL_RenderCopy(renderer, textTexture1, NULL, &textRect1);
                    }
                    else if(myPlayerIndex == 4){
                        textSurface1 = TTF_RenderText_Solid(font, "Left Paddle", textColor);
                        SDL_Texture* textTexture1 = SDL_CreateTextureFromSurface(renderer, textSurface1);    
                        SDL_FreeSurface(textSurface1);
                        SDL_Rect textRect1 ={310, 350, 180, 70 };
                        SDL_RenderCopy(renderer, textTexture1, NULL, &textRect1);
                    }
                }
		        if (!close_pressed)
                {
			        SDL_RenderCopy(renderer, exit_texture, NULL, &close_rect); // Draw the close button image
                }
                    

                if(join_pressed == 1){
                    play_music(music1);
                    printf("Button 'join' pressed\n");
                    p->address.host = (srvadd).host;
                    p->address.port = (srvadd).port; 
                    p->len = strlen(p->data) + 1;
                    SDLNet_UDP_Send(sd, -1, p);
                    join_pressed++;
                }
                if(SDLNet_UDP_Recv(sd, pRecive)){
                    memcpy(&gameData, pRecive->data, sizeof(Data));
                    if(gameData.playerIndex < 5 && gameData.playerIndex >= 1){
                        myPlayerIndex = gameData.playerIndex;
                        printf("MY PLAYER INDEX %d\n", myPlayerIndex);
                    }
                    if(gameData.playerIndex==5){
                        Mix_FreeMusic(music1);
                        play_music(music2);
                        state = 1;
                    }

                }
                break;

                case 1:
                    SDL_RenderCopy(renderer, background_game_texture, NULL, NULL);
                    float x_oldPos = paddles[myPlayerIndex-1]->x;
                    float y_oldPos = paddles[myPlayerIndex-1]->y;
                    drawScore(all_players_info, font, renderer,window);
			        if (check_collision(&ball, paddles[0], paddles[1], paddles[2], paddles[3], all_players_info, renderer, font, window, SoundEffect) == 1) // return 1 när en spelare förlorar
                    {          
                        track++;
                        if (track == 3)
                        {
                            Mix_FreeMusic(music2);
                            play_music(music3);
                        }
                    }
                    int winnerIndex = -1;
                    if (checkWinner(all_players_info, numPlayers, &winnerIndex)) {
                    endGame(renderer, font, window, &winnerIndex, win_texture);
                    break;
		            }
                    update_paddle(paddles[myPlayerIndex-1], 0.018f, myPlayerIndex);                             
                    x_newPos = paddles[myPlayerIndex-1]->x;
                    y_newPos = paddles[myPlayerIndex-1]->y;

                    if(x_oldPos != x_newPos || y_oldPos != y_newPos){
                        if(myPlayerIndex == 1){
                            gameData.downPaddle_x = x_newPos;
                            gameData.downPaddle_y = y_newPos;
                        }else if(myPlayerIndex == 2){
                            gameData.rightPaddle_x = x_newPos;
                            gameData.rightPaddle_y = y_newPos;
                        }else if(myPlayerIndex == 3){
                            gameData.upPaddle_x = x_newPos;
                            gameData.upPaddle_y = y_newPos;
                        }else{
                            gameData.leftPaddle_x = x_newPos;
                            gameData.leftPaddle_y = y_newPos;
                        }
                        gameData.playerIndex = myPlayerIndex;
                        p->address.host = (srvadd).host;
                        p->address.port = (srvadd).port; 
                        memcpy(p->data, &gameData, sizeof(Data));
                        p->len = sizeof(Data);
                        SDLNet_UDP_Send(sd, -1, p);
                        x_oldPos = x_newPos;
                        y_oldPos = y_newPos;
                    }  
                    
                    if(SDLNet_UDP_Recv(sd,pRecive)){                 //receive the data as a client
                        
                        memcpy(&gameData, pRecive->data, sizeof(Data));
                        
                        if(paddles[0]->x != gameData.downPaddle_x || paddles[1]->y != gameData.rightPaddle_y || paddles[2]->x != gameData.upPaddle_x || paddles[3]->y != gameData.leftPaddle_y){
                            moveAllPaddles(paddles, &gameData, myPlayerIndex);
                        } 
                        
                        ball.x = gameData.ball_x;
                        ball.y = gameData.ball_y;
                    }
                    
                    update_objects(&ball, paddles, renderer, 0.018f, myPlayerIndex, &gameData);
                    break;
        }
        SDL_RenderPresent(renderer);
        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
    }
    // Call the cleanupResources function at the end
    cleanupResources(play_button_texture, background_texture, win_texture,background_game_texture, renderer, window, instruction_texture,music3, SoundEffect);
    return 0;
}