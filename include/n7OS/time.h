#ifndef TIME_H
#define TIME_H


//void init_timer();
//void handler_it_timer()



// --------------------------------------
// |    Programmable Interval Timer     |
// --------------------------------------

// Ports 
#define PIT_CH0_PORT 0x40   //Connecté à l’entrée 0 du PIC
#define PIT_CH1_PORT 0x41   //Permet de gérer la mémoire
#define PIT_CH2_PORT 0x42   //Utilisé par le haut parleur du PC
#define PIT_REG_PORT 0x43   //Configurés par un registre de contrôle/commande



// --------------------------------------
// | Programmable Interrupt Controller  |
// --------------------------------------

#define PIC_CMD_PORT 0x20
#define PIC_DATA_PORT 0x21


#define TIMER_IT 0x20





#define FREQUENCE_1kHz 1190 // 1,19 MHz / 1190 = 1 kHz


#endif

