/***** load.c ******/


#include "headers.h"
#include <semaphore.h>


int main(){

	const char *name = "OS";
	int shared_memory;
	int num = 0,size;
	sem_t *mutex = sem_open(SNAME,O_CREAT,0666,1);
	

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
