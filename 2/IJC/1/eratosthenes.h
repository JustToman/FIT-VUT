// eratosthenes.h
// Řešení IJC-DU1, příklad a), 12.3.2020
// Autor: Roman Popelka, FIT
// Přeloženo: gcc 7.5
// Soubor obsahuje prototyp funkce Eratosthenes, která využívá Eratosthenova síta, pro nalezení prvočísel v bitovém poli typu bitset_t
// Implementaci funkce eratosthenes jsou v souboru eratosthenes.c
// Využívá datových typů a funkcí z hlavičkového souboru bitset.h, který pracuje nad bitovými poli

#include "bitset.h"
#include <stdio.h>

/**
 * @brief Pomocí algoritmu Eratosthenova síta, zjišťuje zda jsou čísla prvočísla, či nikoliv
 * Pokud je číslo prvočíslo, nastaví se bit na indexu, který je roven hodnotě daného čísla na 0, pokud číslo
 * prvočíslem není, nastaví se bit na daném indexu na 1
 * 
 * @param pole ukazatel na pole typu bitset_t u kterého se zjišťují prvočísla a nastavují bity
 */
void Eratosthenes(bitset_t pole);