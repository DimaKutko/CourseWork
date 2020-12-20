#ifndef messages_h
#define messages_h

#include <stdio.h>
#include <sys/msg.h>
#include "semaphores.h"
#include "config.h"
#include "numebrs.h"

struct {
    int mtype;
    char mtext[10];
} msg[NUM_COUNT];

int msgId;

void initMsg(){
    key_t key = ftok("queue", 'Q');
    msgId = msgget(key, 0666 | IPC_CREAT);
    
    if(msgId == -1)error("ERROR initMsg()");
}

void disposeMsg(){
    msgctl(msgId, IPC_RMID, 0);
}

int findNextSrc(int srcNum){
    switch (srcNum) {
        case F_SENDER:
            if((SOURCES & 1 << (3 - S_SENDER)) == 0){
                return  S_SENDER;
            }
        case S_SENDER:
            if((SOURCES & 1 << (3 - T_SENDER)) == 0){
                return  T_SENDER;
            }
    }
    
    return  RECEIVER;
}

void sendNums(int srcNum){
    semwait(srcNum);
    
    int* numbers = getNumbers(srcNum);
    int i, snd;
    
    for(i = 0; i < NUM_COUNT; i++){
        sprintf(msg[i].mtext, "%d", numbers[i]);
        msg[i].mtype = srcNum;
        
        snd = msgsnd(msgId, &msg[i], 1 + sizeof(msg[i].mtext), 0);
        
        if(snd == -1) error("ERROR sendNums()");
    }
    
    if(SOURCES & 1 << (3 - srcNum) && srcNum != T_SENDER) return;
    
    sempost(findNextSrc(srcNum));
}

void receiveNums(int srcNum){
    semwait(srcNum);
    int A[NUM_COUNT], B[NUM_COUNT], C[NUM_COUNT], D[NUM_COUNT];
    int i;
    
    for(i = 0; i < NUM_COUNT; i++) {
            msgrcv(msgId, &msg[i], 1 + sizeof(msg[i].mtext), F_SENDER, 0);
            A[i] = atoi(msg[i].mtext);
            msgrcv(msgId, &msg[i], 1 + sizeof(msg[i].mtext), S_SENDER, 0);
            B[i] = atoi(msg[i].mtext);
            msgrcv(msgId, &msg[i], 1 + sizeof(msg[i].mtext), T_SENDER, 0);
            C[i] = atoi(msg[i].mtext);
    
            D[i] = (A[i] + B[i]) * C[i];
            printf("(%d + %d) * %d = %d\n", A[i], B[i], C[i], D[i]);
        }
    
    if(SOURCES & 1 << (3 - srcNum)){
        sendNumbersToFile(srcNum, D);
    }
}

#endif
