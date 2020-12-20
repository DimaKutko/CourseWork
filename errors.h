#ifndef errors_h
#define errors_h

#pragma once

#include <signal.h>
#include <stdio.h>

void error(char* error){
    printf("%s \n", error);
    kill(0, SIGTERM);
}

#endif
