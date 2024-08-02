#include <stdbool.h>

#include <n7OS/mem.h>
#include <n7OS/paging.h>




bool pageTable[MEMORY_SIZE]; // Table de pagination 


int index_page(int addr){
    int index = addr / PAGE_SIZE;
    return index;
}

/**
 * @brief Marque la page allouée
 * 
 * Lorsque la page a été choisie, cette fonction permet de la marquer allouée
 * 
 * @param addr Adresse de la page à allouer
 */
void setPage(uint32_t addr) {
    pageTable[index_page(addr)] = true;
}

/**
 * @brief Désalloue la page
 * 
 * Libère la page allouée.
 * 
 * @param addr Adresse de la page à libérer
 */
void clearPage(uint32_t addr) {
    // L'index de la page est le numéro de la page 
    //     = adresse / taille des pages 
    pageTable[index_page(addr)] = false;

}

/**
 * @brief Fourni la première page libre de la mémoire physique tout en l'allouant
 * 
 * @return uint32_t Adresse de la page sélectionnée
 */
uint32_t findfreePage() {
    for (int i = 0; i < MEMORY_SIZE; i++) {
        //Si la case memoire est libre
        if (pageTable[i] == false) {
            //On l'alloue et on retourn son numéro
            pageTable[i] = true; 
            return i * PAGE_SIZE; 
        } 
    }
    return -1; //Aucune page trouvé
}

/**
 * @brief Initialise le gestionnaire de mémoire physique
 * 
 */
void init_mem() {
    // Initialise toutes les pages vide
    for (int i = 0; i<MEMORY_SIZE; i++) {
        pageTable[i] = false;
    }
}

/**
 * @brief Affiche l'état de la mémoire physique
 * 
 */
void print_mem() {
    printf("  Etats des pages mémoires : ");
    for (uint32_t i = 0; i < MEMORY_SIZE; i++) {
        if(pageTable[i]){
            printf("|-- %d : page allouée\n", i);
        } else {
            printf("|-- %d : page libre\n", i);
        }
    }
}

