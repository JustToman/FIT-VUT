// htab_t.h
// Řešení IJC-DU2, příklad  b) 15.4.2020
// Autor: Roman Popelka, FIT
// Přeloženo: gcc 7.5
#include "htab.h"

struct htab
{
    size_t arr_size;
    size_t size;
    struct htab_item *array[];
};