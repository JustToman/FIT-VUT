// htab_size.c
// Řešení IJC-DU2, příklad  b) 15.4.2020
// Autor: Roman Popelka, FIT
// Přeloženo: gcc 7.5
#include "htab.h"

/**
 * @brief Returns number of records
 * 
 * @param t hast_table
 * @return size
 */
size_t htab_size(const htab_t * t)
{
    return t->size;
}