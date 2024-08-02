#ifndef PROCESS_H
#define PROCESS_H

#include <inttypes.h>

#define STACK_SIZE 1024
#define NB_PROC 511

typedef uint32_t pid_t;
typedef void* (*fnptr)();

//Etat possible d'un processus
typedef enum {
    VIDE,
    BLOQUE,
    PRET,
    ELU,
} PROCESS_ETAT;

//Registre du ctx
typedef enum{
    EBX,
    ESP,
    EBP,
    ESI,
    EDI,
} REGISTRE_CTX;

//Process
typedef struct {
    char nom[50];               //Nom du programme
    pid_t pid;                  //Identifiant du processus
    PROCESS_ETAT etat;          //Etat du processus
    uint32_t stack[STACK_SIZE]; //Pile d'execution
    uint32_t sommet_stack;      
    uint32_t reg_ctx[5];        //Registre du ctx
} t_process;

void scheduler();
pid_t creer(const char *name, fnptr function);
pid_t fork(const char *name, fnptr function);
int exit();
pid_t getpid();
int sleep(int seconds);



#endif