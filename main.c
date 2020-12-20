#include "errors.h"
#include "semaphores.h"
#include "config.h"
#include "messages.h"

#include <sys/types.h>
#include <sys/msg.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

pid_t pid1, pid2, pid3, pid4;


// D - File1
void process1(){
//    printf("PROCESS 1 \n");
    receiveNums(RECEIVER);
}

// A - File2
void process2(){
//    printf("PROCESS 2 \n");
    sendNums(F_SENDER);
}

// B - Console
void process3(){
//    printf("PROCESS 3 \n");
    sendNums(S_SENDER);
}

// C - File4
void process4(){
//    printf("PROCESS 4 \n");
    sendNums(T_SENDER);
}

int main(void) {
    initSem();
    initMsg();
    
    pid1 = getpid();
    pid2 = fork();
    
    if(pid2 == -1) error("ERROR init pid2");
    
    if(pid2 > 0){
        pid4 = fork();
        
        if(pid4 == -1) error("ERROR init pid4");
        
        pid4 > 0 ? process1() : process4();
        
        if(pid4 > 0){
            while (wait(NULL) > 0);
            
            disposeMsg();
            disposeSem();
        }
    }else{
        pid3 = fork();
        
        if(pid3 == -1) error("ERROR init pid3");
        
        pid3 > 0 ? process2() : process3();
    }
    
}
