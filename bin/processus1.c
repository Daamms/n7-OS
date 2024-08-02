#include <n7OS/cpu.h>
#include <stdio.h>
#include <unistd.h>

void processus1(){
    sti();
    printf("\nHello world from proc 1."); 
    for(;;);
}