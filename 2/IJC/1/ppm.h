// ppm.c
// Řešení IJC-DU1, příklad b), 12.3.2020
// Autor: Roman Popelka, FIT
// Přeloženo: gcc 7.5
// Knihovna určená pro načtení dat ze souboru .ppm
// Omezení: Načtení obrázku o maximální velikosti 8000 * 8000 pixelů
#include "error.h"
#include <string.h>

#define BUFFER_SIZE 100 // Velikost bufferu pro načítání zbytku řádku
#define MAX_X_SIZE 8000 //Maximální velikost x-ové souřadnice
#define MAX_Y_SIZE 8000 // Maximální velikost y-ové souřadnice

/**
 * @brief Struktura pro uložení dat z obrázku
 * 
 */
struct ppm{
    unsigned xsize;
    unsigned ysize;
    char *data;
};

struct ppm * ppm_read(const char *filename);
void ppm_free(struct ppm *ppm);
int check_and_load_file_info(FILE *file,struct ppm *ppm);
