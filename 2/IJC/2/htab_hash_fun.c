// htab_hash_fun.c
// Řešení IJC-DU2, příklad  b) 15.4.2020
// Autor: Roman Popelka, FIT
// Přeloženo: gcc 7.5
#include "htab.h"

/**
 * @brief Hashes string to number 
 * 
 * @param str string to be hashed
 * @return size_t 
 */
size_t htab_hash_fun(const char *str) 
{
    uint32_t h=0;
    // musí mít 32 bitů
    const unsigned char *p;
    for(p=(const unsigned char*)str; *p!='\0'; p++)
        h = 65599*h + *p;
    return h;
}