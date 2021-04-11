// htab_iterator_get_key.c
// Řešení IJC-DU2, příklad  b) 15.4.2020
// Autor: Roman Popelka, FIT
// Přeloženo: gcc 7.5
#include "htab.h"

/**
 * @brief Return value of key in iterator
 * 
 * @param it iterator
 * @return htab_key_t  
 */
htab_key_t htab_iterator_get_key(htab_iterator_t it)
{
   return it.ptr->key;
}