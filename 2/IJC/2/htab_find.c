// htab_find.c
// Řešení IJC-DU2, příklad  b) 15.4.2020
// Autor: Roman Popelka, FIT
// Přeloženo: gcc 7.5
#include "htab.h"


/**
 * @brief Finds iterator with matching key
 * 
 * if key is found, returns iterator with matching key
 * if key is not found, returns htab_end
 * 
 * @param t hash_table
 * @param key key to be found
 * @return htab_iterator_t 
 */
htab_iterator_t htab_find(htab_t * t, htab_key_t key)
{
    size_t index =  htab_hash_fun(key) % t->arr_size; // Index calculation
    htab_iterator_t it = {.t = t, .idx = index, .ptr = t->array[index]}; //Setting iterator values
    
    /* Looping through items in given index looking for given key */
    for(;it.ptr != NULL; it.ptr = it.ptr->next)
    {
        if (!strcmp(key,it.ptr->key))  // If iterator key == key returns iterator
            return it;
    }

    /* If key wasn't found, return htab_end*/
    return htab_end(t);  
}