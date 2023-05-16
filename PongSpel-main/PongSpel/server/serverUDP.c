
#define SDL_MAIN_HANDLED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include "SDL2/SDL_net.h"
#include <math.h>

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

float ball_dx = cosf(45 * PI / 180);
float ball_dy = sinf(45 * PI / 180);

void update_ball(Data *gamedata);


int main(int argc, char **argv)
{
	UDPsocket sd;		
	UDPpacket *pRecive; 
	UDPpacket *pSend;

	Uint32 IPclient1 = 0;
	Uint32 IPclient2 = 0;
	Uint32 IPclient3 = 0;
	Uint32 IPclient4 = 0;

	Uint32 portClient1 = 0;
	Uint32 portClient2 = 0;
	Uint32 portClient3 = 0;
	Uint32 portClient4 = 0;

	int state = 0; // 0 is start 1 is ongoing

	int quit;

	Data gameData = {0, 400, 590, 790, 200, 400, 10, 10, 200, 400, 300};
	




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
						if(IPclient1 == 0 && portClient1 == 0){
							printf("Client 1\n");
							IPclient1 = pRecive->address.host;
							portClient1 = pRecive->address.port;
							pSend->address.host = IPclient1;
							pSend->address.port = portClient1;
							gameData.playerIndex = 1;
							memcpy(pSend->data, &gameData, sizeof(Data));
							pSend->len = sizeof(Data);
							SDLNet_UDP_Send(sd, -1, pSend);
							}else if(pRecive->address.port != portClient1  && IPclient2 == 0){								
							printf("Client 2\n");
							IPclient2 = pRecive->address.host;
							portClient2 = pRecive->address.port;
							pSend->address.host = IPclient2;
							pSend->address.port = portClient2;
							gameData.playerIndex = 2;
							memcpy(pSend->data, &gameData, sizeof(Data));
							pSend->len = sizeof(Data);
							SDLNet_UDP_Send(sd, -1, pSend);
							}else if(pRecive->address.port != portClient1 && pRecive->address.port != portClient2 && IPclient3 == 0){
							printf("Client 3\n");
							IPclient3 = pRecive->address.host;
							portClient3 = pRecive->address.port;
							pSend->address.host = IPclient3;
							pSend->address.port = portClient3;
							gameData.playerIndex = 3;
							memcpy(pSend->data, &gameData, sizeof(Data));
							pSend->len = sizeof(Data);
							SDLNet_UDP_Send(sd, -1, pSend);
							}else if(pRecive->address.port != portClient1 && pRecive->address.port != portClient2 && pRecive->address.port != portClient3 && IPclient4 == 0){
							printf("Client 4\n");
							IPclient4 = pRecive->address.host;
							portClient4 = pRecive->address.port;
							pSend->address.host = IPclient4;
							pSend->address.port = portClient4;
							gameData.playerIndex = 4;
							memcpy(pSend->data, &gameData, sizeof(Data));
							pSend->len = sizeof(Data);
							SDLNet_UDP_Send(sd, -1, pSend);
							}
					}
					if(IPclient1 != 0 && IPclient2 != 0 && IPclient3 != 0 && IPclient4 != 0){
						gameData.playerIndex = 5;
						memcpy(pSend->data, &gameData, sizeof(Data));
						pSend->address.host = IPclient1;
						pSend->address.port = portClient1;
						pSend->len = sizeof(Data);
						SDLNet_UDP_Send(sd, -1, pSend);
						pSend->address.host = IPclient2;
						pSend->address.port = portClient2;
						SDLNet_UDP_Send(sd, -1, pSend);
						pSend->address.host = IPclient3;
						pSend->address.port = portClient3;
						SDLNet_UDP_Send(sd, -1, pSend);
						pSend->address.host = IPclient4;
						pSend->address.port = portClient4;
						SDLNet_UDP_Send(sd, -1, pSend);
						state = 1;					
					}
					break;
				case 1:
					
					
					if(SDLNet_UDP_Recv(sd, pRecive)){
					//printf("UDP Packet incoming\n");
					//printf("\tData:    %s\n", pRecive->data);
					//printf("\tAddress: %x %x\n", pRecive->address.host, pRecive->address.port);
					memcpy(&gameData, pRecive->data, sizeof(Data));
					update_ball(&gameData);
						/*if (pRecive->address.port == portClient1){
							//printf("Send to Client 2\n");
							pSend->address.host = IPclient2;	
							pSend->address.port = portClient2;
							
							gameData.playerIndex = 1;
							pSend->len = sizeof(Data) ;
							SDLNet_UDP_Send(sd, -1, pSend);
							
							//printf("Send to Client 3\n");
							pSend->address.host = IPclient3;	
							pSend->address.port = portClient3;
							SDLNet_UDP_Send(sd, -1, pSend);

							//printf("Send to Client 4\n");
							pSend->address.host = IPclient4;	
							pSend->address.port = portClient4;
							SDLNet_UDP_Send(sd, -1, pSend);
							
							} else if (pRecive->address.port == portClient2){
								//printf("Send to Client 1\n");    
								pSend->address.host = IPclient1;	
								pSend->address.port = portClient1;
								gameData.playerIndex = 2;
								memcpy(pSend->data, &gameData, sizeof(Data));
								pSend->len = sizeof(Data);
								SDLNet_UDP_Send(sd, -1, pSend);

								//printf("Send to Client 3\n");
								pSend->address.host = IPclient3;	
								pSend->address.port = portClient3;
								SDLNet_UDP_Send(sd, -1, pSend);
								
								//printf("Send to Client 4\n");
								pSend->address.host = IPclient4;	
								pSend->address.port = portClient4;
								SDLNet_UDP_Send(sd, -1, pSend);

							} else if (pRecive->address.port == portClient3){
								//printf("Send to Client 1\n");    
								pSend->address.host = IPclient1;	
								pSend->address.port = portClient1;
								gameData.playerIndex = 3;
								memcpy(pSend->data, &gameData, sizeof(Data));
								pSend->len = sizeof(Data) ;
								SDLNet_UDP_Send(sd, -1, pSend);

								//printf("Send to Client 2\n");
								pSend->address.host = IPclient2;	
								pSend->address.port = portClient2;
								SDLNet_UDP_Send(sd, -1, pSend);
								
								//printf("Send to Client 4\n");
								pSend->address.host = IPclient4;	
								pSend->address.port = portClient4;
								SDLNet_UDP_Send(sd, -1, pSend);

							}else if (pRecive->address.port == portClient4){
								//printf("Send to Client 1\n");    
								pSend->address.host = IPclient1;	
								pSend->address.port = portClient1;
								gameData.playerIndex = 4;
								memcpy(pSend->data, &gameData, sizeof(Data));
								pSend->len = sizeof(Data);
								SDLNet_UDP_Send(sd, -1, pSend);

								//printf("Send to Client 2\n");
								pSend->address.host = IPclient2;	
								pSend->address.port = portClient2;
								SDLNet_UDP_Send(sd, -1, pSend);
								
								//printf("Send to Client 3\n");
								pSend->address.host = IPclient3;	
								pSend->address.port = portClient3;
								SDLNet_UDP_Send(sd, -1, pSend);
							}
					}else{*/
							//update_ball(&gameData);
							gameData.playerIndex = 5;
							memcpy(pSend->data, &gameData, sizeof(Data));
							pSend->len = sizeof(Data);
							
							pSend->address.host = IPclient1;	
							pSend->address.port = portClient1;
							SDLNet_UDP_Send(sd, -1, pSend);
							pSend->address.host = IPclient2;	
							pSend->address.port = portClient2;
							SDLNet_UDP_Send(sd, -1, pSend);
							pSend->address.host = IPclient3;	
							pSend->address.port = portClient3;
							SDLNet_UDP_Send(sd, -1, pSend);
							pSend->address.host = IPclient4;	
							pSend->address.port = portClient4;
							SDLNet_UDP_Send(sd, -1, pSend);
					}else{
						update_ball(&gameData);
							gameData.playerIndex = 5;
							memcpy(pSend->data, &gameData, sizeof(Data));
							pSend->len = sizeof(Data);
							
							pSend->address.host = IPclient1;	
							pSend->address.port = portClient1;
							SDLNet_UDP_Send(sd, -1, pSend);
							pSend->address.host = IPclient2;	
							pSend->address.port = portClient2;
							SDLNet_UDP_Send(sd, -1, pSend);
							pSend->address.host = IPclient3;	
							pSend->address.port = portClient3;
							SDLNet_UDP_Send(sd, -1, pSend);
							pSend->address.host = IPclient4;	
							pSend->address.port = portClient4;
							SDLNet_UDP_Send(sd, -1, pSend);
					}
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
	float ball_speed = 200;
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
        .x = gamedata->downPaddle_x,
        .y = gamedata->downPaddle_y,
        .w = 120,
        .h = 25,
    };

    SDL_Rect playerRightRect = {
        .x = gamedata->rightPaddle_x,
        .y = gamedata->rightPaddle_y,
        .w = 25,
        .h = 120,
    };

    SDL_Rect playerUpRect = {
        .x = gamedata->upPaddle_x,
        .y = gamedata->upPaddle_y,
        .w = 120,
        .h = 25,
    };

    SDL_Rect playerLeftRect = {
        .x = gamedata->leftPaddle_x,
        .y = gamedata->leftPaddle_y,
        .w = 25,
        .h = 120,
    };

	// check collision with paddles
    if(SDL_HasIntersection(&ballRect, &playerDownRect)){
        if(!lastCollidedDown){
            //printf("Direction change down \n");
            
            ball_dy *= -1;
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
            ball_dx *= -1;
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
            ball_dy *= -1;
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
           ball_dx *= -1;
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