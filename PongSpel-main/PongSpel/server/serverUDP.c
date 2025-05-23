
#define SDL_MAIN_HANDLED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include "SDL2/SDL_net.h"
#include <math.h>
#include <time.h>

#define PI 3.14159265f 

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

int lastCollidedDown = 0;
int lastCollidedUp = 0;
int lastCollidedLeft = 0;
int lastCollidedRight = 0;

int lastCollidedDownWall = 0;
int lastCollidedUpWall = 0;
int lastCollidedLeftWall = 0;
int lastCollidedRightWall = 0;

float degrees; 
float ball_dx; 
float ball_dy; 

void update_ball(Data *gamedata);


int main(int argc, char **argv)
{
	UDPsocket sd;		
	UDPpacket *pRecive; 
	UDPpacket *pSend;


	IPaddress clients[4] = {0};
	int nrOfClients = 0;

	int state = 0; // 0 is start 1 is ongoing

	int quit;

	Data gameData = {0, 400, 590, 400, 10, 790, 200, 10, 200, 400, 300};
	float currBallx, currBally;
    float downPaddlex, downPaddley;
    float rightPaddlex, rightPaddley;
    float upPaddlex, upPaddley;
    float leftPaddlex, leftPaddley;

    time_t t;
    srand((unsigned) time(&t));


    degrees = rand() % 361;
    //printf("start degrees %f\n", degrees);
    ball_dx = cosf(degrees * PI / 180); // 270 up, 90 ner, 0 höger, 180 vänster
    ball_dy = sinf(degrees * PI / 180);
	




	//initializing SDL_net
	if (SDLNet_Init() < 0)
	{
		fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	if (!(sd = SDLNet_UDP_Open(2000)))
	{
		fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	if (!((pSend = SDLNet_AllocPacket(512)) && (pRecive = SDLNet_AllocPacket(512))))
	{
		fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	quit = 0;
	int myPaddleIndex = 0;

	while(!quit){
        frameStart = SDL_GetTicks();
			switch (state){
				case 0:
					
					if(SDLNet_UDP_Recv(sd, pRecive)){
						int exists = 0;
						for(int i=0;i<4;i++){
							if(pRecive->address.host == clients[i].host && pRecive->address.port == clients[i].port){
								exists = 1;
							}
						}
						if(!exists){
							clients[nrOfClients] = pRecive->address;
							nrOfClients++;
							printf("Client %d\n", nrOfClients);
							gameData.playerIndex = nrOfClients;
							memcpy(pSend->data, &gameData, sizeof(Data));
							pSend->address = clients[nrOfClients-1];
							pSend->len = sizeof(Data);
							SDLNet_UDP_Send(sd, -1, pSend);
						}
						
					}
					if(clients[0].host != 0 && clients[1].host != 0 && clients[2].host != 0 && clients[3].host != 0){
						gameData.playerIndex = 5;
						memcpy(pSend->data, &gameData, sizeof(Data));
						pSend->len = sizeof(Data);
						for(int i=0;i<nrOfClients;i++){
							pSend->address = clients[i];
							SDLNet_UDP_Send(sd, -1, pSend);
						}
						state = 1;
					}
					
					break;
				case 1:
	
					if(SDLNet_UDP_Recv(sd, pRecive)){
                    downPaddlex = gameData.downPaddle_x;
                    downPaddley = gameData.downPaddle_y;
                    rightPaddlex = gameData.rightPaddle_x;
                    rightPaddley = gameData.rightPaddle_y;
                    upPaddlex = gameData.upPaddle_x;
                    upPaddley = gameData.upPaddle_y;
                    leftPaddlex = gameData.leftPaddle_x;
                    leftPaddley = gameData.leftPaddle_y;

					memcpy(&gameData, pRecive->data, sizeof(Data));
                    if(gameData.playerIndex == 1){
                        gameData.rightPaddle_x = rightPaddlex;
                        gameData.rightPaddle_y = rightPaddley;
                        gameData.upPaddle_x = upPaddlex;
                        gameData.upPaddle_y = upPaddley;
                        gameData.leftPaddle_x = leftPaddlex;
                        gameData.leftPaddle_y = leftPaddley;
                    } else if(gameData.playerIndex == 2){
                        gameData.downPaddle_x = downPaddlex;
                        gameData.downPaddle_y = downPaddley;
                        gameData.upPaddle_x = upPaddlex;
                        gameData.upPaddle_y = upPaddley;
                        gameData.leftPaddle_x = leftPaddlex;
                        gameData.leftPaddle_y = leftPaddley;
                    } else if(gameData.playerIndex == 3){
                        gameData.rightPaddle_x = rightPaddlex;
                        gameData.rightPaddle_y = rightPaddley;
                        gameData.downPaddle_x = downPaddlex;
                        gameData.downPaddle_y = downPaddley;
                        gameData.leftPaddle_x = leftPaddlex;
                        gameData.leftPaddle_y = leftPaddley;
                    }else if(gameData.playerIndex == 4){ 
                        gameData.rightPaddle_x = rightPaddlex;
                        gameData.rightPaddle_y = rightPaddley;
                        gameData.upPaddle_x = upPaddlex;
                        gameData.upPaddle_y = upPaddley;
                        gameData.downPaddle_x = downPaddlex;
                        gameData.downPaddle_y = downPaddley;
                    }
                
					gameData.ball_x = currBallx;
					gameData.ball_y = currBally;
					update_ball(&gameData);
					memcpy(pSend->data, &gameData, sizeof(Data));
					pSend->len = sizeof(Data);
					for(int i=0;i<nrOfClients;i++){
							pSend->address = clients[i];
							SDLNet_UDP_Send(sd, -1, pSend);
						}

					}else{
						update_ball(&gameData);
						memcpy(pSend->data, &gameData, sizeof(Data));
						pSend->len = sizeof(Data);
						for(int i=0;i<nrOfClients;i++){
							pSend->address = clients[i];
							SDLNet_UDP_Send(sd, -1, pSend);
						}
					}
					currBallx = gameData.ball_x;
					currBally = gameData.ball_y;
					break;
			}
		frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
		
	}
	
	
	
	SDLNet_FreePacket(pSend);
	SDLNet_FreePacket(pRecive);
	SDLNet_Quit();
	return EXIT_SUCCESS;
}

void update_ball(Data *gamedata){
	float delta_time = 0.018;
	float ball_speed = 100;
	float ball_radius = 25;

    
	gamedata->ball_x += ball_dx * ball_speed * delta_time;
	gamedata->ball_y += ball_dy * ball_speed * delta_time;

	// Create kollision detectors
    SDL_Rect ballRect = {
        .x = gamedata->ball_x,
        .y = gamedata->ball_y,
        .w = ball_radius,
        .h = ball_radius,
    };

    SDL_Rect playerDownRect = {
        .x = gamedata->downPaddle_x - 40,
        .y = gamedata->downPaddle_y,
        .w = 120,
        .h = 25,
    };

    SDL_Rect playerRightRect = {
        .x = gamedata->rightPaddle_x,
        .y = gamedata->rightPaddle_y - 40,
        .w = 25,
        .h = 120,
    };

    SDL_Rect playerUpRect = {
        .x = gamedata->upPaddle_x - 40,
        .y = gamedata->upPaddle_y,
        .w = 120,
        .h = 25,
    };

    SDL_Rect playerLeftRect = {
        .x = gamedata->leftPaddle_x,
        .y = gamedata->leftPaddle_y - 40,
        .w = 25,
        .h = 120,
    };

    float degreeFactor1;
    float degreeFactor2 = 140/120;
	// check collision with paddles
    if(SDL_HasIntersection(&ballRect, &playerDownRect)){
        if(!lastCollidedDown){
            //printf("Direction change down \n");
           // printf("%f %f\n", gamedata->ball_x, gamedata->downPaddle_x);
            degreeFactor1 = gamedata->ball_x - gamedata->downPaddle_x + 40;
            if(degreeFactor1<0){
                printf("negative\n");
                degreeFactor1 *= -1;
            } 
            degrees = degreeFactor1 * degreeFactor2 + 200;
            //printf("degreeFactor1: %f\n", degreeFactor1);
            //printf("Degrees: %f\n", degrees);
            ball_dx = cosf(degrees * PI / 180);
            ball_dy = sinf(degrees * PI / 180);
            //ball_dy *= -1;
            lastCollidedDown++;
            lastCollidedDownWall++;
        }            
        lastCollidedLeft = 0;
        lastCollidedRight = 0;
        lastCollidedUp = 0;
        lastCollidedLeftWall = 0;
        lastCollidedRightWall = 0;
        lastCollidedUpWall = 0;
        
    }

    if(SDL_HasIntersection(&ballRect, &playerRightRect)){
        if(!lastCollidedRight){
            //printf("Direction change right\n");
            //printf("%f %f\n", gamedata->ball_y, gamedata->rightPaddle_y);
            degreeFactor1 = gamedata->ball_y - gamedata->rightPaddle_y + 40;
            if(degreeFactor1<0){
                printf("negative\n");
                degreeFactor1 *= -1;
            } 
            degrees = 250 - (degreeFactor1 * degreeFactor2);
            //printf("degreeFactor1: %f\n", degreeFactor1);
            //printf("Degrees: %f\n", degrees);
            ball_dx = cosf(degrees * PI / 180);
            ball_dy = sinf(degrees * PI / 180);
            //ball_dx *= -1;
            lastCollidedRight++;
            lastCollidedRightWall++;
        }
        lastCollidedDown = 0;
        lastCollidedLeft = 0;
        lastCollidedUp = 0;
        lastCollidedDownWall = 0;
        lastCollidedLeftWall = 0;
        lastCollidedUpWall = 0;
        
    }

    if(SDL_HasIntersection(&ballRect, &playerUpRect)){
        if(!lastCollidedUp){
            //printf("Direction change up\n");
            //printf("%f %f\n", gamedata->ball_x, gamedata->upPaddle_x);
            degreeFactor1 = gamedata->ball_x - gamedata->upPaddle_x + 40;
            if(degreeFactor1<0){
                printf("negative\n");
                degreeFactor1 *= -1;
            } 
            degrees = 160 - (degreeFactor1 * degreeFactor2);
            //printf("degreeFactor1: %f\n", degreeFactor1);
            //printf("Degrees: %f\n", degrees);
            ball_dx = cosf(degrees * PI / 180);
            ball_dy = sinf(degrees * PI / 180);
            //ball_dy *= -1;
            lastCollidedUp++;
            lastCollidedUpWall++;
        }
        lastCollidedDown = 0;
        lastCollidedLeft = 0;
        lastCollidedRight = 0;
        lastCollidedDownWall = 0;
        lastCollidedLeftWall = 0;
        lastCollidedRightWall = 0;
    }

    if(SDL_HasIntersection(&ballRect, &playerLeftRect)){
        if(!lastCollidedLeft){
           //printf("Direction change left\n");
            //printf("%f %f\n", gamedata->ball_y, gamedata->leftPaddle_y);
            degreeFactor1 = gamedata->ball_y - gamedata->leftPaddle_y + 40;
            if(degreeFactor1<0){
                printf("negative\n");
                degreeFactor1 *= -1;
            } 
            degrees = 290 + (degreeFactor1 * degreeFactor2);
            //printf("degreeFactor1: %f\n", degreeFactor1);
            //printf("Degrees: %f\n", degrees);
            ball_dx = cosf(degrees * PI / 180);
            ball_dy = sinf(degrees * PI / 180);
           //ball_dx *= -1;
            lastCollidedLeft++;
            lastCollidedLeftWall++;
        }
        lastCollidedDown = 0;
        lastCollidedRight = 0;
        lastCollidedUp = 0;
        lastCollidedDownWall = 0;
        lastCollidedRightWall = 0;
        lastCollidedUpWall = 0;
    }

    // check collision with walls
    if (gamedata->ball_x - ball_radius < 0) {
        if(!lastCollidedLeftWall){
            //printf("Direction change wall left\n");
            ball_dx *= -1;
            lastCollidedLeftWall++;
            lastCollidedLeft++;
        }
        lastCollidedDownWall = 0;
        lastCollidedRightWall = 0;
        lastCollidedUpWall = 0;
        lastCollidedDown = 0;
        lastCollidedRight = 0;
        lastCollidedUp = 0;
    }

    if (gamedata->ball_x + ball_radius > 800) {
        if(!lastCollidedRightWall){
            //printf("Direction change wall right\n");
            ball_dx *= -1;
            lastCollidedRightWall++;
            lastCollidedRight++;
        }
        lastCollidedDownWall = 0;
        lastCollidedLeftWall = 0;
        lastCollidedUpWall = 0;
        lastCollidedDown = 0;
        lastCollidedLeft = 0;
        lastCollidedUp = 0;
    }
    if (gamedata->ball_y - ball_radius < 0) {
        if(!lastCollidedUp){
            //printf("Direction change wall up\n");
            ball_dy *= -1;
            lastCollidedUpWall++;
            lastCollidedUp++;
        }
        lastCollidedDownWall = 0;
        lastCollidedLeftWall = 0;
        lastCollidedRightWall = 0;
        lastCollidedDown = 0;
        lastCollidedLeft = 0;
        lastCollidedRight = 0;
    }
    if (gamedata->ball_y + ball_radius > 600) {
        if(!lastCollidedDownWall){
            //printf("Direction change wall down\n");
            ball_dy *= -1;
            lastCollidedDownWall++;
            lastCollidedDown++;
        }
        lastCollidedUpWall = 0;
        lastCollidedLeftWall = 0;
        lastCollidedRightWall = 0;
        lastCollidedUp = 0;
        lastCollidedLeft = 0;
        lastCollidedRight = 0;
    }
	}
