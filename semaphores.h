#ifndef semaphores_h
#define semaphores_h

#pragma once
#include <sys/sem.h>
#include "errors.h"
#include "config.h"

int semId;
struct sembuf pSembuf, vSembuf;

void initSem()
{
    union semun {
        int val;
        struct semid_ds *buf;
        ushort array[4];
    } sem_args;
    
    key_t key = ftok("semaphores", 'S');
    semId = semget(key, 4, IPC_CREAT | 0777);
    
    if(semId == -1) error("ERROR init_sem()");
    
    int i;
    
    for(i = 3; i >= 0; --i){
        sem_args.array[i] = 0;
        
        if(i != RECEIVER && SOURCES & 1 << i){
            sem_args.array[i] = 1;
        }
    }
    
    while ((i < 3 && sem_args.array[++i]) || i == RECEIVER);
    sem_args.array[i] = 1;
    
    semctl(semId, 0, SETALL, sem_args.array);
    
    pSembuf.sem_op = -1;
    pSembuf.sem_flg = 0;
    
    vSembuf.sem_op = 1;
    vSembuf.sem_flg = 0;
}

void disposeSem(){
    if(semctl(semId, 0, IPC_RMID) == -1){
        error("ERROR remove_sem()");
    }
}

void semwait(int sem_num){
    pSembuf.sem_num = sem_num;
    semop(semId, &pSembuf, 1);
}

void sempost(int sem_num){
    vSembuf.sem_num = sem_num;
    semop(semId, &vSembuf, 1);
}

#endif
