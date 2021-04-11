// htab_iterator_set_value.c
// Řešení IJC-DU2, příklad  b) 15.4.2020
// Autor: Roman Popelka, FIT
// Přeloženo: gcc 7.5
#include "htab.h"

/**
 * @brief Sets iterators item value and returns its value
 * 
 * @param it iterator
 * @param val value to be set
 * @return htab_value_t 
 */
htab_value_t htab_iterator_set_value(htab_iterator_t it, htab_value_t val)
{
    it.ptr->value = val;
    return it.ptr->value;
}