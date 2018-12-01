all:
	gcc -o load load.c -lrt -lpthread
	gcc -o spyGame spyGame.c -lrt -lpthread
