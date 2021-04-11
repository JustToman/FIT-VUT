// htab_end.c
// Řešení IJC-DU2, příklad  b) 15.4.2020
// Autor: Roman Popelka, FIT
// Přeloženo: gcc 7.5
#include "htab.h"


/**
 * @brief Returns iterator to  htab element after last valid element (array[htab->arr_size + 1])
 * 
 * @param t hash_table
 * @return iterator to element after last valid element
 */
htab_iterator_t htab_end(const htab_t * t)
{
    htab_iterator_t iterator = {.t = t, .idx = t->arr_size, .ptr = NULL};  // idx of end is behind array last element
    return iterator;
}