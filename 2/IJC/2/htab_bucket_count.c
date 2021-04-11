// htab_bucket_count.c
// Řešení IJC-DU2, příklad  b) 15.4.2020
// Autor: Roman Popelka, FIT
// Přeloženo: gcc 7.5
#include "htab.h"

/**
 * @brief Returns size of hash tables bucket count
 * 
 * @param t hash table
 * @return size of hash tables bucket count
 */
size_t htab_bucket_count(const htab_t * t)
{
    return t->arr_size;
}