// htab_iterator_get_value.c
// Řešení IJC-DU2, příklad  b) 15.4.2020
// Autor: Roman Popelka, FIT
// Přeloženo: gcc 7.5
#include "htab.h"

/**
 * @brief returns iterator.ptr value
 * 
 * @param it iterator
 * @return htab_value_t 
 */
htab_value_t htab_iterator_get_value(htab_iterator_t it)
{
        return it.ptr->value;
}