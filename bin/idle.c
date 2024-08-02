#include <stdio.h>
#include <n7OS/process.h>
#include <n7OS/cpu.h>



void idle (){
  printf("\nHello world from IDLE");

  scheduler();

  //verification qu'on revient bien dans idle apres process 1
  printf("\nRe from IDLE"); 
  while (1) {
    hlt();
  }
}