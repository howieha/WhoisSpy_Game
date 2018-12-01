/***** spyGame.c ******/


#include "headers.h"
#include <semaphore.h>

int main(){
	const char *name = "OS";
	int shared_memory;
	int size;
	char* lastStr = "hello world\n";
	struct game *spyGame;
	char pid[10];
	snprintf(pid,10,"%d",getpid());
	/* open the shared memory segment as if it was a file */
	shared_memory = shm_open(name, O_RDWR, 0666);
	if (shared_memory == -1) {
		printf("Shared memory failed\n");
		exit(-1);
	}
    
	/* the size of the shared memory segment */
	size = sizeof(struct game);

	/* map the shared memory segment to the address space of the process */
    	spyGame = mmap(0, size, PROT_READ|PROT_WRITE, MAP_SHARED, shared_memory, 0);
    	if (spyGame == MAP_FAILED) {
        	printf("Map failed\n");
        	exit(-1);
    	}
	
	/* load the previously created named semaphore*/
	
	sem_t *mutex = sem_open(SNAME,0);	

	sem_wait(mutex);
	int count = spyGame -> tempCount;
	FILE *inputFile = fopen("word_input.txt","r");
	char role[128];
	if(inputFile != NULL){
		fgets(role,128,inputFile);
		(spyGame-> playerArr[count]).role = 'C';
		if(count == 1){
			fgets(role,128,inputFile);	
			(spyGame-> playerArr[count]).role = 'S';	
		}
		(spyGame-> playerArr[count]).processId = getpid();
		printf("Your word is: %s",role);
		//printf("%d\n",(spyGame-> playerArr[count]).processId);
		//printf("%c\n",(spyGame-> playerArr[count]).role);
		count++;
		spyGame -> tempCount = count;
		if(count == spyGame->numOfPlayer){
			count = 0;
			spyGame -> tempCount = count;	
		}
		//printf("%d\n",spyGame -> tempCount);
	}
	
	
	fclose(inputFile);
	sem_post(mutex);
	while((spyGame -> tempCount) != 0){
		}
	

	
	while(1){
		
		for(int i = 0 ; i < (spyGame -> numOfPlayer); i++){
		(spyGame-> playerArr[i]).numOfVote = 0;	
		}

		char current[128];
		char* currentStr = current;
		char tempLast[50]  = "";
		strcpy(tempLast,lastStr);
		

		while((spyGame -> tempCount) < (spyGame -> numOfPlayer)){
			sem_wait(mutex);
			while((spyGame-> playerArr[spyGame -> tempCount]).role == 'N'){
				(spyGame -> tempCount)++;		
			}
			sem_post(mutex);
			if((spyGame -> tempCount) >= (spyGame -> numOfPlayer))
				break;
		
			while((spyGame-> playerArr[spyGame -> tempCount]).processId != getpid()){
				FILE *inputFile = fopen("/proc/spyLogFile","r");
		
				if(inputFile != NULL){
					if(fgets(currentStr,128,inputFile)){
						/*printf("2: %s",currentStr);*/
						if(strcmp(currentStr,tempLast)!=0){
							
							lastStr = currentStr;
							strcpy(tempLast,"");
							strcpy(tempLast,lastStr);
							printf("%s",currentStr);
						}		
					}
					else{
						fgets(currentStr,128,inputFile);
						/*printf("3: %s",currentStr);*/
						if(strcmp(currentStr,tempLast) != 0){
							lastStr = currentStr;
							strcpy(tempLast,"");
							strcpy(tempLast,lastStr);
							printf("%s",currentStr);
						}
					}
				}
				fseek(inputFile,0,SEEK_SET);
				fclose(inputFile);
				if((spyGame -> tempCount) >= (spyGame -> numOfPlayer)){
					break;
				}
			}
			if((spyGame -> tempCount) >= (spyGame -> numOfPlayer)){
				break;
			}
		
			printf("Concisely describe the word you have: ");

			/* Get input from user */
			char arr[50]="";
			char* tempInput = arr;
			scanf(" %[^\n]s",tempInput);
			char str[50];
			str[0] = '\0';
			strcat(str,pid);
			strcat(str,": ");
			strcat(str,tempInput);
			int inputLength = strlen(tempInput);

			FILE *inputFile2 = fopen("/proc/spyLogFile","r+");
			if(inputFile2 != NULL){
				if(fgets(currentStr,128,inputFile2)){
					/*printf("%s",role);*/		
				}
				else{
					fgets(currentStr,128,inputFile2);
					/*printf("%s",role);*/
				}
			}
		
		
			int difference = strlen(currentStr)-inputLength;
			/*int difference = 5;*/
		
			if(difference>0){
				for(int i = 1;i<difference;i++){
					strcat(str," ");
					/*printf("%s,%d\n",str,strlen(str));*/		
				}	
			}
	
		
			strcat(str,"\n");
			printf("%s",str);
			fputs(str,inputFile2);
			fclose(inputFile2);
			lastStr = str;
			strcpy(tempLast,str);
			sleep(2);
			(spyGame -> tempCount)++;		
		
			/*printf("%d\n",spyGame -> tempCount);
			printf("%d\n",spyGame -> numOfPlayer);*/
		
		}
		sleep(2);
		spyGame -> tempCount = 0;
		sleep(1);

		

		/* Show the valid player IDs that are qualified to be voted */
		printf("Players (ID) that can vote and be voted: ");
		for(int i = 0 ; i < (spyGame -> numOfPlayer); i++){
			if(((spyGame-> playerArr[i]).role)!='N'){
				printf("%d, ",(spyGame-> playerArr[i]).processId);
			}			
		}
		printf("\n");
		
		/* Time to vote */
		printf("Now please vote out a player with his ID (PID): ");
		int voteId = 0;

		/* if ID incorrect, ask the user for input again until it's a valid ID*/
		int correctID = 0;
		while(!correctID){
			scanf("%d",&voteId);
			for(int i = 0 ; i < (spyGame -> numOfPlayer); i++){
				if((voteId == (spyGame-> playerArr[i]).processId) && ((spyGame-> playerArr[i]).role)!='N'){
					correctID = 1;
					break;
				}			
			}
			if(!correctID){
				printf("Invalid player ID, please give another input again: ");			
			}
		}
		
		sem_wait(mutex);
		for(int i = 0 ; i < (spyGame -> numOfPlayer); i++){
			if(voteId == (spyGame-> playerArr[i]).processId){
				((spyGame-> playerArr[i]).numOfVote)++;
				(spyGame -> tempCount) ++;			
				if((spyGame -> tempCount) == (spyGame -> numOfPlayerAlived)){
					spyGame -> tempCount = 0;			
				}		
				break;		
			}	
		}
		sem_post(mutex);
		while(spyGame -> tempCount != 0){
		}
		sleep(2);	

		/* Get the voting result */
		int maxId = 0;
		int maxVote = 0;
	
		sem_wait(mutex);
		for(int i = 0 ; i < (spyGame -> numOfPlayer); i++){
			if((spyGame-> playerArr[i]).role != 'N'){
				printf("Vote for player %d: %d\n",(spyGame-> playerArr[i]).processId,(spyGame-> playerArr[i]).numOfVote);
				if((spyGame-> playerArr[i]).numOfVote > maxVote){
					maxVote = (spyGame-> playerArr[i]).numOfVote;
					maxId = (spyGame-> playerArr[i]).processId;
				}
			}	
		}
		(spyGame -> tempCount)++;
		if((spyGame -> tempCount) == (spyGame -> numOfPlayerAlived)){
				spyGame -> tempCount = 0;
		}
		sem_post(mutex);
		while(spyGame -> tempCount != 0){
		}
	
		sleep(2);
		
		/* Remove that player from the game, and check if termination condition meets */



		sem_wait(mutex);
		if(getpid() == maxId){
			for(int i = 0 ; i < (spyGame -> numOfPlayer); i++){
				if((spyGame-> playerArr[i]).processId == maxId){
					(spyGame-> playerArr[i]).role = 'N';
					printf("You have been voted out of the game.\n");
					printf("You lose.\n");
					(spyGame -> numOfPlayerAlived)--;
					(spyGame -> tempCount)++;
					sem_post(mutex);
					exit(0);
				}		
			}
			
		}
		else{
			printf("Player number %d has been voted out of the game\n", maxId);
			(spyGame -> tempCount)++;
			//printf("%d\n",spyGame -> tempCount);	
		}		
		sem_post(mutex);

		while((spyGame -> tempCount)!=((spyGame -> numOfPlayerAlived)+1)){

			
		}

		sleep(2);
		(spyGame -> tempCount) = 0;
		
		if((spyGame-> playerArr[1]).role == 'N'){
			if(getpid()!=((spyGame-> playerArr[1]).processId)){
				printf("Congratulations on your win!\n");
				exit(0);
			}
		}
		else if(spyGame -> numOfPlayerAlived <= 2){
			if(getpid()==((spyGame-> playerArr[1]).processId)){
				printf("Congratulations on your win!\n");
			}
			else{
				printf("You lose. \n");
			}
			exit(0);
		}
		else{
			printf("The game continues... \n");

		}
		sleep(2);
	
	}
	return 0;
}


