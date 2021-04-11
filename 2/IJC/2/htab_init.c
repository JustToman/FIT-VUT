// htab_init.c
// Řešení IJC-DU2, příklad  b) 15.4.2020
// Autor: Roman Popelka, FIT
// Přeloženo: gcc 7.5
#include "htab.h"


/**
 * @brief Htab constructor
 * 
 * @param n number of elements in  htab->array
 * @return htab_t* allocated hash_table
 */
htab_t *htab_init(size_t n)
{
    /*Allocating sizeof(htab) + (n+1) * sizeof(htab_item) for items.. +1 because of htab_end*/
    htab_t *htab = (htab_t *)malloc(sizeof(htab_t) + (n+1) * sizeof(struct htab_item));       
    
    /* Allocation check */
    if(htab == NULL)
        return htab;

    /* htab initialization */
    htab->arr_size = n;
    htab->size = 0;

    for(size_t i = 0; i <= htab->arr_size; i++)
    {
        htab->array[i] = NULL;
    }
    
    return htab;
}