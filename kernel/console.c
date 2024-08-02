#include <n7OS/console.h>
#include <n7OS/time.h>
#include <n7OS/cpu.h>

#define POS_MAX VGA_WIDTH*VGA_HEIGHT

// Clignote pas (0) | fond gris (111) | char noir (0000)
#define COLOR_HEAD 0b01110000
// Clignote pas (0) | fond noir (000) | char gris (0111)
#define COLOR_BODY 0b00000111

uint16_t *scr_tab;  // Tableau représentant la console
int pos;
char color;    // Format d'affichage (clignote - clouleur de fon - couleur de char)



// FONCTIONS


/* 
    Initialise l'affichage de la console avec un valeur par defaut 
*/
void init_console() {
    
    color = COLOR_BODY;
    scr_tab = (uint16_t *) SCREEN_ADDR;
    pos = 0;

    clear_screen();
    pos = 80;
}


/* 
    Place le curseur 
*/
void set_cursor(){
    outb(CMD_LOW          , PORT_CMD);  // Selectionne les bits de poids faible
    outb(pos & 0xFF       , PORT_DATA); // Envoie les bits de poids faible de la position
    outb(CMD_HIGH         , PORT_CMD);  // Selectionne les bits de poids fort
    outb((pos >> 8) & 0xFF, PORT_DATA); // Envoie les bits de fort faible de la position
}


/* 
    Agie sur la console selon le char en entrée : affiche ou déplace le curseur
    in : (char) caractere à afficher ou commande
*/
void console_putchar(const char c) {
    // Caractere à afficher
    if(c >= 32 && c < 127){ // Affiche un caracetere dans la console
        if(pos > POS_MAX)
            scroll();
        scr_tab[pos++]= (color<<8) | c;

    // 8 : Backspace (BS) 
    } else if (c == '\b' ) { // Déplace le curseur d’une colonne en arrière
        pos--;
    
    // 9 : Horizontal Tab (HT)
    } else if (c == '\t' ) { // Ajoute un espace de 8 caractères
        pos += 8;
    
    // 10 : Line Feed (LF)
    } else if (c == '\n' ) { // Saut de ligne 
        pos = pos - (pos % VGA_WIDTH) + VGA_WIDTH; 
        if(pos > POS_MAX)
            pos = POS_MAX;
     
    // 12 : Form Feed(FF)
    } else if (c == '\f' ) { // Efface l’écran et revient à la colonne 0 de la ligne 0
        pos = 0;
        clear_screen();
    
    // 13 : Carriage Return (CR)
    } else if (c == '\r' ) { // Déplace le curseur à la colonne 0 de la ligne courante
        pos = pos - (pos % VGA_WIDTH);
    }
}


/* 
    Parcour la chaîne et affiche les char un par un.
    in : (char*) string à afficher
        (int)   taille de la string 
    out : -                                           
*/
void console_putbytes(const char *s, int len) {
    for (int i= 0; i<len; i++) {
        console_putchar(s[i]);
    }
}

/*
    Affiche une chaine à un endroit voulu
*/
void console_putbytes_at(const char *s, int len, int p) {
    int pos_old = pos;

    pos = p;
    for (int i= 0; i<len; i++) {
        console_putchar(s[i]);
    }

    pos = pos_old;
}

/*
    Définie la position du curseur
*/
void set_pos(int p){
    if(p<POS_MAX)
        pos = p;
}


// AFFICHAGE

/*
    Réinitialise l'ecran vide, avec le titre et l'heure uniquement
*/
void clear_screen(){
    for(int i = VGA_WIDTH; i < POS_MAX; i++){
        scr_tab[i]= (color<<8);
    }

    put_header();
    pos = 80;
}


/*

*/
void scroll(){
    //Remplie les lignes du caractère d'en dessous
    for(int i=VGA_WIDTH; i<VGA_WIDTH*(VGA_HEIGHT-1); i++){
        scr_tab[i] = scr_tab[(i) + VGA_WIDTH];
    }

    //Nettoie la dernière ligne
    for(int i = VGA_WIDTH*(VGA_HEIGHT-1); i < VGA_WIDTH*VGA_HEIGHT; i++){
        scr_tab[i]= (color<<8);
    }
    pos -= VGA_WIDTH;
}


/*
    Affiche le bandeau contenant le titre et l'heure
*/
void put_header(){
    color = COLOR_HEAD;
    //Titre
    console_putbytes_at("  n7OS by Damien B.                                                    _  |_| X ",80,0);
    //Heure
    put_time();
    color = COLOR_BODY;
}


/*
    Afficher l'heure en haut à doite du bandeau
*/
void put_time(){
    //Récupère le temps en format HH:MM:SS
    int t = get_time();
    char buf[8];
    sprintf(buf, "%02i:%02i:%02i",  t / 3600000 //Heure
                                 , (t/60000)%60 //Minute
                                 , (t/1000)%60);//Seconde
    //Affiche l'heure sur les 7 char en haut à droite
    color = COLOR_HEAD;
    console_putbytes_at(buf, 8, VGA_WIDTH - 8 - 11);    
    color = COLOR_BODY;
}



