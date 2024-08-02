#include <n7OS/time.h>
#include <n7OS/console.h>
#include <n7OS/cpu.h>
#include <n7OS/process.h>


extern void handler_IT_timer();
extern int get_time();
extern int time;
extern t_process* table_process;
extern int* table_attente;


int time = 1;

/*
    Renvoie le temps en ms depuis le lancement de l'OS
    (utilisé principalement par "put_time()" de la consolle)
*/
int get_time(){
    return time;
}

// (1) Ecrire la fonction qui initialise le timer pour la fréquence de 1
//kHz et la ligne d’IT correspondante (penser à avoir un fichier
//assembleur de traitement de l’IT) ;
void init_timer()
{
    //Configuration du PIT : Channel 0
    //                       Accès poids faible/poids fort,
    //                       Générateur d’impulsion
    //                       Fréquence définie en binaire
    outb(0x34, PIT_REG_PORT);

    //Fréquence timer = 1kHz
    outb(FREQUENCE_1kHz & 0xFF, PIT_CH0_PORT);
    outb(FREQUENCE_1kHz >> 8, PIT_CH0_PORT);
    
    //Enregistrement du handler
    init_irq_entry(TIMER_IT, (uint32_t)handler_IT_timer);
    
    //Demasquage de l'IT
    outb(inb(PIC_DATA_PORT) & 0xfe, PIC_DATA_PORT);
}



// (2) Écrire en C la fonction de traitement de l’IT reçue. Cette
//fonction acquitte l’IT et incrémente le compteur système.
void handler_it_timer()
{
    outb(TIMER_IT, TIMER_IT);   //Acquittement de l'IT 
    time++;                     //Incrémente le temps de 1 ms
    
    sti();

    if(time%1000 == 0){
        put_time();             //Affiche le nouveau temps dans la console
        
        //Reveil les processus
        for(int i=0;i<NB_PROC; i++){
            if(table_attente[i] == time)
                table_process[i].etat = ELU;
        }
        
        scheduler();            //Change le processus en cours
    }

}





