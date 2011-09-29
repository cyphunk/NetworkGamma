// nathan@squimp.com do whatever you like with this
// expects byte type of 0-127 ranges for r,g,b gamma
#include <stdio.h>

#include <Carbon/Carbon.h>
#include <ApplicationServices/ApplicationServices.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/types.h>

#include "main.h"

int sock;

void sigcatch(int sig) {
	printf("You have pressed Ctrl-C , please press again to exit");
	if (sock != -1) close(sock);	
	CGDisplayForceToGray(false);
	(void) signal(SIGINT, SIG_DFL);
}

int main (int argc, const char * argv[]) {
	CGError err;		
	CGGammaValue redMin, redMax, redGamma,
	greenMin, greenMax, greenGamma,
	blueMin, blueMax, blueGamma;
	
	printf("setting up connection on UDP port 4740\n");
	printf("server accepts 3 values of 0 to 127 that\n");
	printf("are applied to the screen.\n");
	
	// Current Gamma settings:
	err = CGGetDisplayTransferByFormula (0,
										 &redMin, &redMax, &redGamma,
										 &greenMin, &greenMax, &greenGamma,
										 &blueMin, &blueMax, &blueGamma);
	
	(void) signal(SIGINT, sigcatch);

	// Setup network:
	int port = 4740;
	sock = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
	if ( sock <= 0 ) {
		printf( "failed to create socket\n" );
		return false;
	}
	sockaddr_in address;
	memset(&address, 0, sizeof(address));		
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( (unsigned short) port );
	if ( bind( sock, (const struct sockaddr *)&address, sizeof(sockaddr_in) ) < 0 ) {
		printf( "failed to bind socket\n" );
		return false;
	}

	// just for the hell of it
	CGDisplayForceToGray(true);
	
	while ( true ) // wait for packet
	{
		sockaddr_in from;
		socklen_t fromLength = sizeof( from );		
		gammaFractions gammaPacket;
		// process received packet
		int rxbytes=recvfrom(sock,(char*)&gammaPacket, sizeof(gammaPacket),0,(const struct sockaddr*)&from,&fromLength);
		if ( rxbytes <= 0 )
			continue;
		
		//unsigned int from_address = ntohl( from.sin_addr.s_addr );
		//unsigned int from_port = ntohs( from.sin_port );

		printf("recieved packet, bytes: %d\n", rxbytes);
		printf("red: %x, green: %x, blue: %x\n", gammaPacket.red, gammaPacket.green, gammaPacket.blue);
		err = CGSetDisplayTransferByFormula (0,
											 redMin,   ((1.0/127)*gammaPacket.red  )*redMax,   redGamma,
											 greenMin, ((1.0/127)*gammaPacket.green)*greenMax, greenGamma,
											 blueMin,  ((1.0/127)*gammaPacket.blue )*blueMax,  blueGamma);		
	}

    return 0;
}

