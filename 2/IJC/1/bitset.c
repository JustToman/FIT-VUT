// bitset.c
// Řešení IJC-DU1, příklad a), 12.3.2020
// Autor: Roman Popelka, FIT
// Přeloženo: gcc 7.5
// Definice maker, inline funkcí,  popisy funkcí a popis knihovny, se nacházejí v souboru bitset.h
#include "bitset.h"

 extern inline void bitset_free(bitset_t jmeno_pole);
 extern inline unsigned long bitset_size(bitset_t jmeno_pole);
 extern inline int bitset_getbit(bitset_t jmeno_pole, bitset_index_t index);
 extern inline void bitset_setbit(bitset_t jmeno_pole, bitset_index_t index, unsigned long vyraz);
