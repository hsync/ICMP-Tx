#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "icmptx.h"


#define DST_IP "192.168.0.10"


int main (int argc, char **argv)
{
	int uid;
	char msg[50];
	// application start as root?
	uid =  getuid();
	if(uid != 0)
	{
		printf("You must be root, to run this program\n");
		exit(1);
	}
	if(argc > 1)
	{
		char *msg = new char[strlen(argv[1])];
		strcpy(msg, argv[1]);
		icmptx packet(DST_IP);
		packet.sendPacket(msg, strlen(msg));

	}
	else
	{
		printf("No message\n");
		exit(1);
	}
	
	

	return 0;
}


