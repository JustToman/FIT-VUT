// htab_begin.c
// Řešení IJC-DU2, příklad  b) 15.4.2020
// Autor: Roman Popelka, FIT
// Přeloženo: gcc 7.5
#include "htab.h"

/**
 * @brief Return iterator to beginning element
 * 
 * @param t hash_table
 * @return htab_iterator_t to beginning element 
 */
htab_iterator_t htab_begin(const htab_t * t)
{
   htab_iterator_t iterator = {.t = t, .idx = 0, .ptr = t->array[0]};
   return iterator;  
}