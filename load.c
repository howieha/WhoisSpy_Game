/***** load.c ******/


#include "headers.h"
#include <semaphore.h>

void drawSpyInit(){
	for (int i = 0;i<90;i++){
		printf("-");
	}
	printf("\n");
	for (int i = 0;i<90;i++){
		printf("-");
	}	
	for (int i = 0;i<3;i++){
		printf("\n");	
	}
	
	for (int i = 0;i<=20;i++){
		if(i==0){
			printf("     ");
			for(int j = 0;j<20;j++){
				printf("-");			
			}
			printf("     ");
			for(int j = 0;j<20;j++){
				printf("-");			
			}
			printf("     ");
			printf("-");
			printf("                  ");
			printf("-");
			printf("     \n");       
	
		}
		else if(i<10){
			printf("     ");
			printf("|");
			printf("                        ");
			printf("|");
			printf("                  ");
			printf("|");
			for(int j = 0;j<(5+i);j++){
				printf(" ");			
			}		
			printf("-");
			for(int j = 0;j<(9-i)*2;j++){
				printf(" ");			
			}		
			printf("-");
			for(int j = 0;j<(5+i-1);j++){
				printf(" ");			
			}
			printf("\n");
		}	
		else if(i==10){
			printf("     ");
			for(int j = 0;j<20;j++){
				printf("-");			
			}
			printf("     ");
			for(int j = 0;j<20;j++){
				printf("-");			
			}
			printf("               ");
			printf("|");
			printf("              \n");
		}
		else if(i<20){
			printf("                        ");
			printf("|");
			printf("     ");
			printf("|");
			printf("                                  ");
			printf("|");
			printf("              \n");
		}
		else{
			printf("     ");
			for(int j = 0;j<20;j++){
				printf("-");			
			}
			printf("     ");
			printf("|");
			printf("                                  ");
			printf("|");
			printf("              \n");		
		}
	}	
	
	printf("\n");
	for (int i = 0;i<90;i++){
		printf("-");
	}
	printf("\n");
	for (int i = 0;i<90;i++){
		printf("-");
	}	
	printf("\n");

}


int main(){

	const char *name = "OS";
	int shared_memory;
	int num = 0,size;
	sem_t *mutex = sem_open(SNAME,O_CREAT,0666,1);
	
	drawSpyInit();
	printf("How many players in total(>= 3 && <= 5): ");
	scanf("%d",&num);
	
	struct game spyGame;
	struct game *ptr;

	/* create the shared memory segment */
        shared_memory = shm_open(name, O_CREAT|O_RDWR, 0666);	
    
	if (shared_memory == -1) {
            	printf("Shared memory failed\n");
            	exit(-1);
        }
	
	/* configure the size of the shared memory segment */
        size = sizeof(spyGame);
	
        ftruncate(shared_memory, size);
	
	/* now map the shared memory segment in the address space of the process */
        ptr = mmap(0, size, PROT_READ|PROT_WRITE, MAP_SHARED, shared_memory, 0);
        if (ptr == MAP_FAILED) {
            printf("Map failed\n");
            exit(-1);
        }
	
	
	ptr -> numOfPlayer = num;
	ptr -> numOfPlayerAlived = num;
	for (int i = 0;i < 5;i++){
		
		(ptr-> playerArr[i]).role = 'N';
		(ptr-> playerArr[i]).numOfVote = 0;
	}
	ptr -> tempCount = 0;
	
	

	return 0;
}


