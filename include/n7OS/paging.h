/**
 * @file paging.h
 * @brief Gestion de la pagination dans le noyau
 */
#ifndef _PAGING_H
#define _PAGING_H

#include <inttypes.h>
//#include <stdbool.h>

extern void loadPageDirectory(unsigned int* dir);
extern void enablePaging();

#define PAGE_PRESENT 1
#define PAGE_RW      2
#define PAGE_USER    4
#define PAGE_DIRTY   16

/**
 * @brief Description d'une ligne de la table de page
 * 
 */
typedef struct {
    uint32_t present   :  1; //=1 : page présente en mémoire
    uint32_t rw        :  1; //=1 : page accessible en lecture/écriture (0 : lecture seule)
    uint32_t user      :  1; //=1 : page utilisateur ; =0 : page en mode noyau
    uint32_t accessed  :  1; //Accessed bit
    uint32_t dirty     :  1; //D : Dirty bit (page modifiée)
    uint32_t unused    :  7;
    uint32_t page      : 20; //Page : Adresse de la page en mémoire physique (20 bits)
} page_table_entry_t;


/**
 * @brief Une entrée dans la table de page peut être manipulée en utilisant
 *        la structure page_table_entry_t ou directement la valeur
 */
typedef union {
    page_table_entry_t page_entry;
    uint32_t value;
} PTE; // PTE = Page Table Entry 

/**
 * @brief Une table de page (PageTable) est un tableau de descripteurs de page
 * 
 */
typedef PTE * PageTable;



typedef struct {
    uint32_t present   :  1; //Table présente en mémoire
    uint32_t rw        :  1; //1 => table de page accessible en lecture/écriture, 0 => lecture uniquement
    uint32_t user      :  1; //1 => page utilisateur, 0 => page en mode noyau
    uint32_t reserved  :  9; //Bits reservés 
    uint32_t page      : 20; //Adresse de la page en mémoire physique
} page_dir_entry_t;

typedef union {
    page_dir_entry_t dir_entry;
    uint32_t value;
} PDE;

typedef PDE * PageDirectory;



/**
 * @brief Cette fonction initialise le répertoire de page, alloue les pages de table du noyau
 *        et active la pagination
 * 
 */
void initialise_paging();

/**
 * @brief Cette fonction alloue une page de la mémoire physique à une adresse de la mémoire virtuelle
 * 
 * @param address       Adresse de la mémoire virtuelle à mapper
 * @param is_writeable  Si is_writeable == 1, la page est accessible en écriture
 * @param is_kernel     Si is_kernel == 1, la page ne peut être accédée que par le noyau
 * @return PageTable    La table de page modifiée
 */
PageTable alloc_page_entry(uint32_t address, int is_writeable, int is_kernel);
#endif