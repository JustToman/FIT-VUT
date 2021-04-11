// htab_erase.c
// Řešení IJC-DU2, příklad  b) 15.4.2020
// Autor: Roman Popelka, FIT
// Přeloženo: gcc 7.5
#include "htab.h"


/**
 * @brief Frees iterator->ptr, moves iterator to next element
 * 
 * @param t hash table
 * @param it iterator to be erase
 */
void htab_erase(htab_t * t, htab_iterator_t it)
{
    struct htab_item * item_prev;
   
    if(it.ptr != NULL && it.ptr != t->array[it.idx]) {
    for(item_prev = t->array[it.idx]; item_prev->next != it.ptr; item_prev = item_prev->next);

    item_prev->next = it.ptr->next;

    free((char *)it.ptr->key);
    free(it.ptr);
  
    }
    else if( it.ptr != NULL && it.ptr == t->array[it.idx])
    {
        t->array[it.idx] = it.ptr->next;
        free((char *)it.ptr);
        free(it.ptr);
    }
}