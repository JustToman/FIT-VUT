// htab_free.c
// Řešení IJC-DU2, příklad  b) 15.4.2020
// Autor: Roman Popelka, FIT
// Přeloženo: gcc 7.5
#include "htab.h"


/**
 * @brief Frees hash table
 * 
 * Calls htab_clear
 * 
 * @param t 
 */
void htab_free(htab_t * t)
{
    htab_clear(t);
    free(t);
}