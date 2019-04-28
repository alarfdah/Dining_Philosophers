all: deadlock_recovery deadlock starvation

starvation: starvation.c
	gcc -Wall -g -o starvation starvation.c -lpthread

deadlock: deadlock.c
	gcc -Wall -g -o deadlock deadlock.c -lpthread

deadlock_recovery: deadlock_recovery.c
	gcc -Wall -g -o deadlock_recovery deadlock_recovery.c -lpthread