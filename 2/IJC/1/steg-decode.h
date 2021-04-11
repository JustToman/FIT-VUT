// steg-decode.c
// Řešení IJC-DU1, příklad b), 12.3.2020
// Autor: Roman Popelka, FIT
// Přeloženo: gcc 7.5
// Slouží pro nalezení zakódované zprávy v datech načtených z obrázku formátu .ppm
// Pro načtení využívá knihovny ppm.h, pro práci s bity využívá knihovny bitset.h, pro hledání prvočísel využívá implementace eratosthenova
// síta s knihovny eratosthenes.h
// Implementace funkcí je v souboru steg_decode.c
#include "eratosthenes.h"
#include "ppm.h"

#define SIZE_CHAR sizeof(char)*BITS_IN_BYTE  // Velikost charu v bitech

/**
 * @brief Načte LSB ze jmeno_pole na indexu index
 * 
 */
#define char_getbit(jmeno_pole, index) (((jmeno_pole[index]) & (1)) ? 1:0)

/**
 * @brief Nastaví bit v poli charů na indexu "index" na hodnotu zadaného výrazu
 * 
 */
#define char_setbit(jmeno_pole, index, vyraz)  if((vyraz)) jmeno_pole[index / (SIZE_CHAR)] |= (1 << (index % (SIZE_CHAR)));\
                                               if(!(vyraz)) jmeno_pole[index / (SIZE_CHAR)] &= ~(1 << (index % (SIZE_CHAR)))
/**
 * @brief Zjistí počet prvočísel v bitovém poli
 * 
 * @param pole bitové pole
 * @return unsigned long - počet prvočísel
 */
unsigned long get_n_of_primes(const bitset_t pole);

/**
 * @brief Zjistí zakódovanou "tajnou" zprávu a načte ji do pole charů předaného v argumentech
 * 
 * @param message pole charů do kterého se zpráva uloží
 * @param ppm Struktura obsahující parametry a data z obrázku
 * @param pole pole bitů obsahující informace o tom, zda je číslo na zadaném indexu prvočíslo nebo ne
 */
void get_message(unsigned char *message,const struct ppm *ppm,const bitset_t pole);