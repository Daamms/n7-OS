#include <inttypes.h>
#include <n7OS/irq.h>


// irq_num = num de l'interuption en entrée
// addr    = address du handler
void init_irq_entry(int irq_num, uint32_t addr) {

    idt_entry_t *idt_entry = (idt_entry_t *)&idt[irq_num];

    // Init 
    idt_entry->offset_inf  = addr & 0xFFFF;
    idt_entry->sel_segment = KERNEL_CS;
    idt_entry->zero        = 0;
    idt_entry->type_attr   = 0x8E;
    idt_entry->offset_sup  = (addr >> 16) & 0xFFFF;

}
