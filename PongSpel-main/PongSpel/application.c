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

Mix_Chunk *SoundEffect = Mix_LoadWAV("src/missBall.wav");
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

void run_application()
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
        return;
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
                    // Draw the Play button image
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
                // Render the text every frame as long as joinTextShown is set to 1
                if(join_pressed){
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
                        
                        SDL_Surface* textSurface1 = TTF_RenderText_Solid(font, "Up Paddle", textColor);
                        SDL_Texture* textTexture1 = SDL_CreateTextureFromSurface(renderer, textSurface1);    
                        SDL_FreeSurface(textSurface1);
                        SDL_Rect textRect1 ={310, 350, 180, 70 };
                        SDL_RenderCopy(renderer, textTexture1, NULL, &textRect1);
                    }
                    else if(myPlayerIndex == 4){
                        SDL_Surface* textSurface1 = TTF_RenderText_Solid(font, "Left Paddle", textColor);
                        SDL_Texture* textTexture1 = SDL_CreateTextureFromSurface(renderer, textSurface1);    
                        SDL_FreeSurface(textSurface1);
                        SDL_Rect textRect1 ={310, 350, 180, 70 };
                        SDL_RenderCopy(renderer, textTexture1, NULL, &textRect1);
                    }
                }
		        if (!close_pressed)
                {
			        // Draw the close button image
			        SDL_RenderCopy(renderer, exit_texture, NULL, &close_rect);
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
                    //quit = 1;
                    endGame(renderer, font, window, &winnerIndex, win_texture);
                    break;
		            }
                    update_paddle(paddles[myPlayerIndex-1], 0.018f, myPlayerIndex);                             
                    x_newPos = paddles[myPlayerIndex-1]->x;
                    y_newPos = paddles[myPlayerIndex-1]->y;

                    if(x_oldPos != x_newPos || y_oldPos != y_newPos){
                        //printf("Flag TO SEND -----------\n");
                        //printf("New: %f %f Old: %f %f\n", x_newPos, y_newPos, x_oldPos, y_newPos);
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
                        
                        //printf("Flag RECIEVED\n");
                        
                        memcpy(&gameData, pRecive->data, sizeof(Data));
                        /*printf("Data: %d %f %f %f %f %f %f %f %f %f %f\n", gameData.playerIndex, gameData.downPaddle_x, 
                        gameData.downPaddle_y, gameData.leftPaddle_x, gameData.leftPaddle_y, gameData.rightPaddle_x, 
                        gameData.rightPaddle_y, gameData.upPaddle_x, gameData.upPaddle_y, gameData.ball_x, gameData.ball_y);*/
                        
                        if(paddles[0]->x != gameData.downPaddle_x || paddles[1]->y != gameData.rightPaddle_y || paddles[2]->x != gameData.upPaddle_x || paddles[3]->y != gameData.leftPaddle_y){
                            moveAllPaddles(paddles, &gameData, myPlayerIndex);
                        } 
                        
                        ball.x = gameData.ball_x;
                        ball.y = gameData.ball_y;
                        /*render_ball(&ball, renderer);
                        render_paddle(paddles[0], renderer);
                        render_paddle(paddles[1], renderer);
                        render_paddle(paddles[2], renderer);
                        render_paddle(paddles[3], renderer);*/
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

    // Free game objects
    /*destroy_ball(&ball);
    for(int i=0; i<4 ; i++){
        destroy_paddle(Paddle* paddle[i]);
    }*/
    // Free resources
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
