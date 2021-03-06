 
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include "config.h"

int semId;
int shmId;

typedef struct sembuf sembuf;

void boxOrder()
{
    sembuf * toBox = calloc(4, sizeof(sembuf));

    toBox[0].sem_num = 0;
    toBox[0].sem_op = 0;
    toBox[0].sem_flg = 0;

    toBox[1].sem_num = 0;
    toBox[1].sem_op = 1;
    toBox[1].sem_flg = 0;

    toBox[2].sem_num = 2;
    toBox[2].sem_op = 1;
    toBox[2].sem_flg = 0;

    toBox[3].sem_num = 3;
    toBox[3].sem_op = -1;
    toBox[3].sem_flg = 0;

    semop(semId, toBox, 4);

    orders* orders = shmat(shmId, NULL, 0);
    int idx = (semctl(semId, 2, GETVAL, NULL) -1) % MAX_ORDERS;
    orders->values[idx] *= 2;


    int ordersToPrepare = semctl(semId, 3, GETVAL, NULL);
    int ordersToSend = semctl(semId, 5, GETVAL, NULL) + 1;
    
     printf("%d %ld: Przygotowalem zamowienie o wielkosci: %d. Liczba zamowien do przygotowania: %d. Liczba zamowien do wyslania: %d.\n",
           getpid(), time(NULL), orders->values[idx], ordersToPrepare, ordersToSend);

    shmdt(orders);

    sembuf *returned = calloc(2, sizeof(sembuf));

    returned[0].sem_num = 0;
    returned[0].sem_op = -1;
    returned[0].sem_flg = 0;

    returned[1].sem_num = 5;
    returned[1].sem_op = 1;
    returned[1].sem_flg = 0; 

    semop(semId, returned, 2);

}

int main()
{
    srand(time(NULL));
    semId = getSemaphore();
    shmId = getSharedMemory();

    while(1)
    {
        usleep(randTime());
        if(semctl(semId, 3, GETVAL, NULL) > 0)
            boxOrder();
    }
    return 0;
}
