#include <n7OS/cpu.h>
#include <n7OS/processor_structs.h>
#include <n7OS/process.h>
#include <n7OS/console.h>
#include <inttypes.h>
#include <unistd.h>
#include <stdio.h>

extern void handler_50();
extern void idle();
extern void processus1();


void kernel_start(void)
{
    setup_base(0 /* la memoire virtuelle n'est pas encore definie */);
    
    init_console();
    init_timer();
    init_process();


    // lancement des interruptions
    sti();


    /*uint32_t pagedir = initialise_paging();
    setup_base(pagedir);#include <processus1.h>
    printf("0x%x\n", pagedir);*/
    


    //Init les IT
    //init_irq_entry(50, (uint32_t)handler_50);
    //__asm__("int $50"::);

  

    /*uint32_t *ptr = (uint32_t*)0x10000;
    uint32_t do_page_fault = *ptr;
    do_page_fault++;
    */


    // on ne doit jamais sortir de kernel_start
    while (1) {
        // cette fonction arrete le processeur
        hlt();
    }
}
