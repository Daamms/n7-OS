#include <n7OS/process.h>
#include <n7OS/cpu.h>
#include <string.h>
#include <stdio.h>


extern void idle();
extern void processus1();

//Processus courant 
t_process* process_courant;

// Table des processus
t_process table_process[NB_PROC];

//Tableau des processus en attente
int table_attente[NB_PROC];

void init_process(){
    //S'assure que toute la table de processus est vide
    for(int i=0; i < NB_PROC; i++){
        table_process[i].etat = VIDE;
    }

    //Creation de nos deux processus 
    //Processus de base, actif
    process_courant = &table_process[fork("idle", (fnptr)idle)];
    process_courant->etat = ELU;

    //Processus en attente
    //fork("idle", (fnptr)idle);
    fork("proc 1", (fnptr)processus1);

    sti();
    idle();
}



//   Fonction permettant de sauvegarder les registres de la pile d'un processus puis 
// de lire les registres de la pile du deuxième processus
extern void ctx_sw(void *ctx_old, void *ctx_new);

// Crée un processus et l'ajoute à la table de processus
pid_t fork(const char *name, fnptr function)
{
    t_process * process;

    //Trouve la premiere place libre de la table de processus
    for (int i = 0; i < NB_PROC; i++) {


        // Si la case est libre, on initialise le processus avec ces parametre
        if (table_process[i].etat == VIDE) {
            process = &table_process[i];

            strcpy(process->nom, name);    //Nom
            process->pid = i;      //PID
            process->etat = PRET;          //Etat 
            memset(process->stack, 0, sizeof(process->stack));      //Mise à 0 de la stack 
            process->stack[STACK_SIZE - 1] = (uint32_t)function;    //  avec la fonction à la fin
            process->sommet_stack = STACK_SIZE - 1;
            memset(process->reg_ctx, 0, sizeof(process->reg_ctx));          //Mise à 0 du ctx
            
            process->reg_ctx[ESP] = (uint32_t)&process->stack[STACK_SIZE - 1];// on place le PC
        
            printf("\nNouveau processus cree : \"%s\" (pid=%i", process->nom, process->pid);
            if(process->etat == PRET){ printf(", etat=PRET)");}
            else if(process->etat == ELU){ printf(", etat=ELU)");}
            else if(process->etat == VIDE){ printf(", etat=VIDE)");}
            else if(process->etat == BLOQUE){ printf(", etat=BLOQUE)");}
            
            return process->pid;
        }
    }

    printf("\nProcessus %s non cree car il n'y a plus de place dans la table de processus", name);
    return -1;
}


void scheduler() {
    pid_t new_pid  = -1;        //Nouveau pid à -1 pour si il n'est pas trouvé
    pid_t curr_pid = getpid();

    printf("\n> Scheduler");

    // Recherche du procahin processus en attente de lancement 
    for (pid_t i = 1; i < NB_PROC+1; i++) {
        // On cherche à partir du processus actuel et on repart au début un fois le tableau fini (equité)
        pid_t npid = (curr_pid + i) % NB_PROC;

        if (table_process[npid].etat == PRET) {
            new_pid = npid;
            break;
        }
    }

    // Si il y a un processus en attente, on le lance
    if (new_pid != -1) {

        // Si un processus tournais, on le met en attente pour le relancer
        if (process_courant->etat == ELU)
            process_courant->etat = PRET;

        // On lance le nouveau processus
        table_process[new_pid].etat = ELU;
        process_courant = &table_process[new_pid];
        printf("\n | Nouveau processus en cours : %s", process_courant->nom);

        ctx_sw(&table_process[curr_pid].reg_ctx, &table_process[new_pid].reg_ctx);
    }
        // Si il n'y a pas de processus en attente, on relancele processus actuel
    else{
        printf("\n | Aucun processus en attente, \"%s\" continue", process_courant->nom);
    }
}



int exit(){
    // Arret du processus
    process_courant->etat = VIDE;
    printf("\nFin du processus %s\n", process_courant->nom);

    //Lancement du processus suivant
    scheduler();
}


int sleep(int t){
    // Arret du processus
    process_courant->etat = PRET;
    printf("\nArreter du processus %s\n pour %i secondes", process_courant->nom, t);

    //Dans la case adapté, fait une attente de t seconde 
    table_attente[process_courant->pid] = get_time()/1000 + 1000*t;

    //Lancement du processus suivant
    scheduler();
}

pid_t getpid(){
    return process_courant->pid;
}





