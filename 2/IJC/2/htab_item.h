// htab_item.h
// Řešení IJC-DU2, příklad  b) 15.4.2020
// Autor: Roman Popelka, FIT
// Přeloženo: gcc 7.5
#include "htab.h"

struct htab_item
{
    const char * key;
    int value;
    struct htab_item *next;
};