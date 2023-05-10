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

Mix_Music *load_music()
{
    Mix_Music *music = Mix_LoadMUS("Cyberpunk_Moonlight_Sonata.mp3");
    if (!music)
    {
        SDL_Log("Failed to load music: %s", Mix_GetError());
        return NULL;
    }
    return music;
}

int play_music(Mix_Music *music)
{
    if (Mix_PlayMusic(music, -1) < 0)
    {
        SDL_Log("Failed to play music: %s", Mix_GetError());
        return 0;
    }
    return 1;
}

SDL_Window *create_window()
{
    return SDL_CreateWindow("My Pong Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
}

SDL_Renderer *create_renderer(SDL_Window *window)
{
    return SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}


void set_render_draw_color(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}
////////////////////////// vi ligger alla obj (4 paddle med olika positioner och ball i skärmen)
void initialize_game_objects(SDL_Renderer *renderer, Ball *ball, Paddle **paddles)
{
    initialize_ball(ball, renderer);
    paddles[0] = initialize_paddle(renderer);
    paddles[1] = initialize_paddle2(renderer);
    paddles[2] = initialize_paddle3(renderer);
    paddles[3] = initialize_paddle4(renderer);
}

// för ladda alla objs bilder i skärmen ?
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

void set_play_button_rect(SDL_Window *window, SDL_Rect *play_button_rect)
{
    SDL_GetWindowSize(window, &play_button_rect->x, &play_button_rect->y);
    play_button_rect->x /= 2;
    play_button_rect->y /= 2;
    play_button_rect->x -= play_button_rect->w / 2;
    play_button_rect->y -= play_button_rect->h / 2;
}

void handle_events(SDL_Event event, int *quit, int *play_button_pressed, SDL_Rect play_button_rect)
{
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            *quit = 1;
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN)
        {
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

void update_objects(Ball *ball, Paddle *paddles[], SDL_Renderer *renderer, float deltaTime, int playerIndex)
{
    // Update and render the ball
    //update_ball(ball, paddles[0], paddles[1], paddles[2], paddles[3], deltaTime);
    render_ball(ball, renderer);

    // Update and render the paddles
    update_paddle(paddles[0], paddles[1], paddles[2], paddles[3], deltaTime, playerIndex);
    render_paddle(paddles[0], renderer);
    render_paddle(paddles[1], renderer);
    render_paddle(paddles[2], renderer);
    render_paddle(paddles[3], renderer);
}



void moveAllPaddles (Paddle* paddles[], int index, float p1, float p2)
{
    //printf("Flag 3 ----------- %d\n", index);
    if(index == 1)
    {
        paddles[0]->x = p1;
        paddles[0]->y = p2;
        //printf("Moving to this %f %f\n", p1, p2);
    }
    else if(index == 2)
     {
        paddles[1]->x = p1;
        paddles[1]->y = p2;
        //printf("x and y index 2: %f %f\n", paddles[1]->x, paddles[1]->y);
     }
     else if(index == 3)
     {
        paddles[2]->x = p1;
        paddles[2]->y = p2;
        //printf("Index is: %d\n", index);
     }
     else if(index == 4)
    {
        paddles[3]->x = p1;
        paddles[3]->y = p2;
        //printf("Index is: %d\n", index);
    }

}

void run_application()
{

    UDPsocket sd;
	IPaddress srvadd;
    srvadd.port = 2000;
	UDPpacket *p;
    UDPpacket *pRecieve;
    int recieveID;
    int playerIndex;
    int state = 0; // 0 is start 1 is ongoing
 
    float x_newPos;
    float y_newPos;
    int playerCount = 1;
    initialize_sdl();
    initialize_sdl_mixer();

  

  
  if (SDLNet_Init() < 0)            
	{
		fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());           
		exit(EXIT_FAILURE);                                                
	} 

    if (!(sd = SDLNet_UDP_Open(0)))         // open a UDP socket 
	{
		fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());  
        printf("called from main");
		exit(EXIT_FAILURE);
	}

     
	if (SDLNet_ResolveHost(&srvadd, "127.0.0.1", 2000) == -1)
	{
		fprintf(stderr, "SDLNet_ResolveHost(192.0.0.1 2000): %s\n", SDLNet_GetError());  // an error message is printed, If the host cannot be resolved
		exit(EXIT_FAILURE);    
	}


    if (!((p = SDLNet_AllocPacket(512))&& (pRecieve = SDLNet_AllocPacket(512))))    // two packets are allocated, one for sending data and one for receiving data, packet size 512
	{
		fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

 

    Mix_Music *music = load_music();
    if (!music)
    {
        SDL_Log("Failed to load music");
    }
    if (!play_music(music))
    {
        SDL_Log("Failed to play music");
    }

    SDL_Window *window = create_window();
    SDL_Renderer *renderer = create_renderer(window);

    set_render_draw_color(renderer);

    Ball ball;
    Paddle *paddles[4];
    initialize_game_objects(renderer, &ball, paddles);

    SDL_Texture *play_button_texture = load_texture(renderer, "play.png");
    SDL_Texture *game_over_texture = load_texture(renderer, "game_over.jpg");

    // Set the Play button position and size
    SDL_Rect play_button_rect;
    SDL_QueryTexture(play_button_texture, NULL, NULL, &play_button_rect.w, &play_button_rect.h);
    set_play_button_rect(window, &play_button_rect);




      if (TTF_Init() != 0) {                                 //använder SDL_ttf-biblioteket för att initiera ett TrueType-teckensnitt och ladda det från en teckensnittsfil på disken.
        printf("TTF_Init() failed: %s\n", SDL_GetError());
        
    }

    TTF_Font* font = TTF_OpenFont("C:\\Windows\\Fonts\\arial.ttf", 16);  // för att ladda ett TrueType-teckensnitt från en teckensnittsfil på disken.
        if (!font) {
            SDL_Log("Failed to load font: %s", TTF_GetError());
            return;
        }

    ///////////////////////////////////////-2-
    int quit = 0;
    int play_button_pressed = 0;
    int game_over = 0;
    int track = 0;
    int otherPaddleIndex;
    float timer = 5; 


    //int all_players_points[4]={0};           // Array för att lagra alla spelarens points 
    Player all_players_info[4];                // struct för att lagra alla spelarens info  
    initPlayers(all_players_info);              // To Set alla plyers score to zero frön början 

    printScore(renderer, font, all_players_info, window);

    while (!quit) 
    {
        timer = timer + 0.01666667;
        frameStart = SDL_GetTicks();
        SDL_Event event;
        switch (state){
            case 0:
                // Move,render the ball and the paddle
                Mix_HaltMusic();
                // Stänger ner ljudsystemet
                Mix_CloseAudio();
                handle_events(event, &quit, &play_button_pressed, play_button_rect);
                SDL_RenderClear(renderer);
                //printf("%d", play_button_pressed);
                if (!play_button_pressed)
                {
                    // Draw the Play button image
                    SDL_RenderCopy(renderer, play_button_texture, NULL, &play_button_rect);

                }
                SDL_RenderPresent(renderer);

                if(play_button_pressed == 1){
                    printf("IM HERE");
                    p->address.host = (srvadd).host;
                    p->address.port = (srvadd).port; 
                    p->len = strlen((char *)p->data) + 1;
                    SDLNet_UDP_Send(sd, -1, p);
                    play_button_pressed++;
                }
                if(SDLNet_UDP_Recv(sd, pRecieve)){
                    int message;
                    sscanf((char *)pRecieve->data, "%d", &message);
                    printf("MESSAGE %d", message);
                    if(message < 5 && message >= 1){
                        playerIndex = message;
                        printf("MY PLAYER INDEX %d\n", playerIndex);
                    }
                    if(message==5){
                        state = 1;
                    }

                }
                break;

                case 1:
                    float x_oldPos = paddles[playerIndex-1]->x;
                    float y_oldPos = paddles[playerIndex-1]->y;

                    handle_events(event, &quit, &play_button_pressed, play_button_rect);
                    SDL_RenderClear(renderer);

                    update_objects(&ball, paddles, renderer, 0.018f, playerIndex);

                    if (!game_over)
                    {
                        

                        

                        if (check_collision(&ball, paddles[0], paddles[1], paddles[2], paddles[3], all_players_info, renderer, font, window) == 1)     // return 1 när en spelare förlorar
                        {       
                                
                            track++;
                            if (track == 3)
                            {
                                // Stoppar musiken
                                
                            }
                        }
                    
                    } if (track == 3)
            
                        {
                            SDL_Rect game_over_rect = {0, 0, 300, 100};
                            SDL_GetWindowSize(window, &game_over_rect.w, &game_over_rect.h);
                            game_over_rect.x = (game_over_rect.w - game_over_rect.w) / 2;
                            game_over_rect.y = (game_over_rect.h - game_over_rect.h) / 2;

                            SDL_RenderCopy(renderer, game_over_texture, NULL, &game_over_rect);
                            game_over =1; 
                        
                        }

                    SDL_RenderPresent(renderer);


                    x_newPos = paddles[playerIndex-1]->x;
                    y_newPos = paddles[playerIndex-1]->y;
                    if(x_oldPos != x_newPos || y_oldPos != y_newPos){

                        printf("Flag TO SEND -----------\n");
                        //the host will send this data
                        printf("New: %f %f Old: %f %f\n", x_newPos, y_newPos, x_oldPos, y_newPos);

                        sprintf((char *)p->data, "%d %f %f \n",playerIndex, x_newPos, y_newPos);
                        p->address.host = (srvadd).host;
                        p->address.port = (srvadd).port; 
                        p->len = strlen((char *)p->data) + 1;
                        SDLNet_UDP_Send(sd, -1, p);
                        x_oldPos = x_newPos;
                        y_oldPos = y_newPos;
                    }  
                    
                    if(SDLNet_UDP_Recv(sd,pRecieve)){                 //receive the data as a client
                        //printf("Flag 1 -----------");
                        int otherPlayerIndex;
                        float otherPlayerX, otherPlayerY;
                        sscanf((char *)pRecieve->data, "%d %f %f",&otherPlayerIndex, &otherPlayerX, &otherPlayerY);   
                        printf("Flag RECIEVED %d %f %f \n",otherPlayerIndex, otherPlayerX, otherPlayerY);
                        moveAllPaddles(paddles, otherPlayerIndex, otherPlayerX, otherPlayerY);
                        render_ball(&ball, renderer);
                        render_paddle(paddles[0], renderer);
                        render_paddle(paddles[1], renderer);
                        render_paddle(paddles[2], renderer);
                        render_paddle(paddles[3], renderer);
                    }

        }

        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
        
        
        //printf("Flag 2 --------------------------- ");
        
    }

    // Free game objects
    destroy_ball(&ball);
    // destroy_paddle(paddles[0], paddles[1], paddles[2], paddles[3]);

    // Free resources
    SDL_DestroyTexture(game_over_texture);
    SDL_DestroyTexture(play_button_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_FreeMusic(music);
    Mix_CloseAudio();
    SDL_Quit();
}
