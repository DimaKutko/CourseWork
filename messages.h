#ifndef messages_h
#define messages_h

#include <stdio.h>
#include <sys/msg.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include "semaphores.h"
#include "config.h"
#include "numebrs.h"

struct {
    long mtype;
    char Data[6];
} Message;

int msgId;

void initMsg(){
    key_t key = ftok("queue", 'Q');
    msgId = msgget(key, 0666 | IPC_CREAT );
    
    if(msgId == -1) error("ERROR initMsg()");
}

void disposeMsg(){
    printf("disposeMsg");
    msgctl(msgId, IPC_RMID, 0);
}

int findNextSrc(int srcNum){
    switch (srcNum) {
        case F_SENDER:
            if((SOURCES & 1 << (3 - S_SENDER)) == 0){
                return S_SENDER;
            }
        case S_SENDER:
            if((SOURCES & 1 << (3 - T_SENDER)) == 0){
                return T_SENDER;
            }
    }
    
    return RECEIVER;
}

void sendNums(int srcNum){
    semwait(srcNum);
    
    int* numbers = getNumbers(srcNum);
    int i, snd;
    int isFile = SOURCES & 1 << (3 - srcNum);
    
    for (i = 0; i < NUM_COUNT; ++i) {
        
        sprintf(Message.Data, "%d", numbers[i]);
        
        Message.mtype = srcNum;
        
        snd = msgsnd(msgId, (struct msgbuf *) &Message, 1 + strlen(Message.Data), 0);
        
        if(snd == -1) error("ERROR snd");
        
    }
    
    if (srcNum != T_SENDER && isFile) return;
    
    int src_next = findNextSrc(srcNum);
    
    sempost(src_next);
}

void receiveNums(int srcNum){
    semwait(srcNum);
    
    int A[NUM_COUNT], B[NUM_COUNT], C[NUM_COUNT], D[NUM_COUNT];
    int i;
    
    for(i = 0; i < NUM_COUNT; i++) {
        msgrcv(msgId, (struct msgbuf *) &Message, 1+strlen(Message.Data), F_SENDER, 0);
        A[i] = atoi(Message.Data);
        msgrcv(msgId, (struct msgbuf *) &Message, 1+strlen(Message.Data), S_SENDER, 0);
        B[i] = atoi(Message.Data);
        msgrcv(msgId, (struct msgbuf *) &Message, 1+strlen(Message.Data), T_SENDER, 0);
        C[i] = atoi(Message.Data);
        
        D[i] = (A[i] + B[i]) * C[i];
        printf("(%d + %d) * %d = %d\n", A[i], B[i], C[i], D[i]);
    }
    
    if(SOURCES & 1 << (3 - srcNum)){
        sendNumbersToFile(srcNum, D);
    }
}

#endif
