// htab_clear.c
// Řešení IJC-DU2, příklad  b) 15.4.2020
// Autor: Roman Popelka, FIT
// Přeloženo: gcc 7.5
#include "htab.h"

/**
 * @brief Frees all hash tab elements
 * 
 * @param t hash_tab
 */
void htab_clear(htab_t * t)
{
    /* Outter loop is for each array element*/
    for(size_t i = 0; i < t->arr_size; i++)
    {
        struct htab_item *tmp_next;
        /* Inner loop is for each item */
        for(struct htab_item *tmp = t->array[i]; tmp != NULL; tmp = tmp_next)
        {
            tmp_next = tmp->next;
            free((char *)tmp->key);
            free(tmp);
        }
    }

}