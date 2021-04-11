// htab_lookup_add.c
// Řešení IJC-DU2, příklad  b) 15.4.2020
// Autor: Roman Popelka, FIT
// Přeloženo: gcc 7.5
#include "htab.h"
#include <stdio.h>
/**
 * @brief Search for item with key equal to given key
 * 
 * If key is found returns iterator to this item
 * IF key is not found creates item with this key and returns iterator
 * If malloc is unsucessful it.ptr = NULL
 * 
 * @param t hash table
 * @param key key
 * @return htab_iterator_t 
 */
htab_iterator_t htab_lookup_add(htab_t *t, htab_key_t key)
{
    size_t index = htab_hash_fun(key) % t->arr_size; //Calculate htab index
    htab_iterator_t it = {.t = t, .idx = index,.ptr =  t->array[index]}; //sets iterator values
    
    struct htab_item * prev_item = NULL;

    for(;it.ptr != NULL; it.ptr = it.ptr->next)
    {
        if (!strcmp(key,it.ptr->key)) //If key was already added, returns iterator
            return it;

        /*If it.ptr->next == NULL, then this is the item, which will point to new allocated item*/
        if (it.ptr->next == NULL) 
             prev_item=it.ptr;   
    }

    struct htab_item * new_item; // this variable will point to new allocated item

    /* If t->array[it.idx] wasn't allocated, we need to allocate new_item and then store it to t->array[it.idx]*/
    if( t->array[it.idx] == NULL )
    {
        if ((new_item = malloc(sizeof(struct htab_item))) == NULL )
            return it;
        t->array[it.idx] = new_item;
    }
    else if( (new_item = malloc(sizeof(struct htab_item))) == NULL)
        return it;
    
    /* Setting new_item values */
    if((new_item->key = malloc(MAX_WORD_LEN)) == NULL)
    {
        free(new_item);
        return it;
    }

    new_item->value = 0;
    new_item->next = NULL;

    strcpy((char *)new_item->key,key);

    /* setting prev_item->next pointer to new_item*/
    if(new_item != t->array[it.idx])
        prev_item->next = new_item;

    /* Setting it.ptr pointer to new item */
    it.ptr = new_item;

    /* Incrementing t->size */
    t->size++;
    return it;
}
