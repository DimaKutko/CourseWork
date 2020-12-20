#include "errors.h"
#include "semaphores.h"
#include "config.h"
#include "messages.h"

pid_t pid1, pid2, pid3, pid4;

// D - File1
void process1(){
    receiveNums(RECEIVER);
}

// A - File2
void process2(){
    sendNums(F_SENDER);
}

// B - Console
void process3(){
    sendNums(S_SENDER);
}

// C - File4
void process4(){
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
