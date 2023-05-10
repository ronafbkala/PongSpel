
#define SDL_MAIN_HANDLED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SDL2/SDL_net.h"

int main(int argc, char **argv)
{
	UDPsocket sd;		
	UDPpacket *pRecive; 
	UDPpacket *pSent;

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

	float a,b;
	int index;


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

	if (!((pSent = SDLNet_AllocPacket(512)) && (pRecive = SDLNet_AllocPacket(512))))
	{
		fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	quit = 0;
	int myPaddleIndex = 0;

	while(!quit){
			switch (state){
				case 0:
					
					if(SDLNet_UDP_Recv(sd, pRecive)){
						if(IPclient1 == 0 && portClient1 == 0){
							printf("Client 1\n");
							IPclient1 = pRecive->address.host;
							portClient1 = pRecive->address.port;
							pSent->address.host = IPclient1;
							pSent->address.port = portClient1;
							sprintf((char *)pSent->data,"%d\n", 1);
							pSent->len = strlen((char *)pSent->data) + 1;
							SDLNet_UDP_Send(sd, -1, pSent);
							}else if(pRecive->address.port != portClient1  && IPclient2 == 0){								
							printf("Client 2\n");
							IPclient2 = pRecive->address.host;
							portClient2 = pRecive->address.port;
							pSent->address.host = IPclient2;
							pSent->address.port = portClient2;
							sprintf((char *)pSent->data,"%d\n", 2);
							pSent->len = strlen((char *)pSent->data) + 1;
							SDLNet_UDP_Send(sd, -1, pSent);
							}else if(pRecive->address.port != portClient1 && pRecive->address.port != portClient2 && IPclient3 == 0){
							printf("Client 3\n");
							IPclient3 = pRecive->address.host;
							portClient3 = pRecive->address.port;
							pSent->address.host = IPclient3;
							pSent->address.port = portClient3;
							sprintf((char *)pSent->data,"%d\n", 3);
							pSent->len = strlen((char *)pSent->data) + 1;
							SDLNet_UDP_Send(sd, -1, pSent);
							}else if(pRecive->address.port != portClient1 && pRecive->address.port != portClient2 && pRecive->address.port != portClient3 && IPclient4 == 0){
							printf("Client 4\n");
							IPclient4 = pRecive->address.host;
							portClient4 = pRecive->address.port;
							pSent->address.host = IPclient4;
							pSent->address.port = portClient4;
							sprintf((char *)pSent->data,"%d\n", 4);
							pSent->len = strlen((char *)pSent->data) + 1;
							SDLNet_UDP_Send(sd, -1, pSent);
							}
					}
					if(IPclient1 != 0 && IPclient2 != 0 && IPclient3 != 0 && IPclient4 != 0){
						pSent->address.host = IPclient1;
						pSent->address.port = portClient1;
						sprintf((char *)pSent->data,"%d\n", 5);
						pSent->len = strlen((char *)pSent->data) + 1;
						SDLNet_UDP_Send(sd, -1, pSent);
						pSent->address.host = IPclient2;
						pSent->address.port = portClient2;
						SDLNet_UDP_Send(sd, -1, pSent);
						pSent->address.host = IPclient3;
						pSent->address.port = portClient3;
						SDLNet_UDP_Send(sd, -1, pSent);
						pSent->address.host = IPclient4;
						pSent->address.port = portClient4;
						SDLNet_UDP_Send(sd, -1, pSent);
						state = 1;					
					}
					break;
				case 1:
						if(SDLNet_UDP_Recv(sd, pRecive)){
						//printf("UDP Packet incoming\n");
						//printf("\tData:    %s\n", (char *)pRecive->data);
						//printf("\tAddress: %x %x\n", pRecive->address.host, pRecive->address.port);
							if (pRecive->address.port == portClient1){
								//printf("Recived data\n");
								printf("Send to Client 2\n");
								pSent->address.host = IPclient2;	/* Set the destination host */
								pSent->address.port = portClient2;
								sscanf((char * )pRecive->data, "%d %f %f\n",&index, &a, &b);
								sprintf((char *)pSent->data, "%d %f %f\n",index, a,  b);
								pSent->len = strlen((char *)pSent->data) + 1;
								SDLNet_UDP_Send(sd, -1, pSent);
								
								printf("Send to Client 3\n");
								pSent->address.host = IPclient3;	/* Set the destination host */
								pSent->address.port = portClient3;
								SDLNet_UDP_Send(sd, -1, pSent);

								printf("Send to Client 4\n");
								pSent->address.host = IPclient4;	/* Set the destination host */
								pSent->address.port = portClient4;
								SDLNet_UDP_Send(sd, -1, pSent);
								
								} else if (pRecive->address.port == portClient2){
									printf("Send to Client 1\n");    
									pSent->address.host = IPclient1;	/* Set the destination host */
									pSent->address.port = portClient1;
									sscanf((char * )pRecive->data, "%d %f %f\n",&index, &a, &b);
									sprintf((char *)pSent->data, "%d %f %f\n",index, a,  b);
									pSent->len = strlen((char *)pSent->data) + 1;
									SDLNet_UDP_Send(sd, -1, pSent);

									printf("Send to Client 3\n");
									pSent->address.host = IPclient3;	/* Set the destination host */
									pSent->address.port = portClient3;
									SDLNet_UDP_Send(sd, -1, pSent);
									
									printf("Send to Client 4\n");
									pSent->address.host = IPclient4;	/* Set the destination host */
									pSent->address.port = portClient4;
									SDLNet_UDP_Send(sd, -1, pSent);

								} else if (pRecive->address.port == portClient3){
									printf("Send to Client 1\n");    
									pSent->address.host = IPclient1;	/* Set the destination host */
									pSent->address.port = portClient1;
									sscanf((char * )pRecive->data, "%d %f %f\n",&index, &a, &b);
									sprintf((char *)pSent->data, "%d %f %f\n",index, a,  b);
									pSent->len = strlen((char *)pSent->data) + 1;
									SDLNet_UDP_Send(sd, -1, pSent);

									printf("Send to Client 2\n");
									pSent->address.host = IPclient2;	/* Set the destination host */
									pSent->address.port = portClient2;
									SDLNet_UDP_Send(sd, -1, pSent);
									
									printf("Send to Client 4\n");
									pSent->address.host = IPclient4;	/* Set the destination host */
									pSent->address.port = portClient4;
									SDLNet_UDP_Send(sd, -1, pSent);

								}else if (pRecive->address.port == portClient4){
									printf("Send to Client 1\n");    
									pSent->address.host = IPclient1;	/* Set the destination host */
									pSent->address.port = portClient1;
									sscanf((char * )pRecive->data, "%d %f %f\n",&index, &a, &b);
									sprintf((char *)pSent->data, "%d %f %f\n",index, a,  b);
									pSent->len = strlen((char *)pSent->data) + 1;
									SDLNet_UDP_Send(sd, -1, pSent);

									printf("Send to Client 2\n");
									pSent->address.host = IPclient2;	/* Set the destination host */
									pSent->address.port = portClient2;
									SDLNet_UDP_Send(sd, -1, pSent);
									
									printf("Send to Client 3\n");
									pSent->address.host = IPclient3;	/* Set the destination host */
									pSent->address.port = portClient3;
									SDLNet_UDP_Send(sd, -1, pSent);
								}
						}else{
								
						}
						break;
			}
		
		if (strcmp((char *)pSent->data, "quit") == 0)
				quit = 1;
	}
	
	
	SDLNet_FreePacket(pSent);
	SDLNet_FreePacket(pRecive);
	SDLNet_Quit();
	return EXIT_SUCCESS;
}
