/***** headers.h ******/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>

# define SNAME "AIK"

struct player {
	int processId;                           	// process id
	char role;                         		// role (N/S/C)  N for null, S for spy, C for civilian
	int numOfVote;					// number of votes the player received
};

struct game{
	int numOfPlayer;				// number of player
	int numOfPlayerAlived;				// number of alived player
	int tempCount;					// do counting in the game
	struct player playerArr[5];			// player info array
};



