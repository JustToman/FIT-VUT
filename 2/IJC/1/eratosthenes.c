// eratosthenes.c
// Řešení IJC-DU1, příklad a), 12.3.2020
// Autor: Roman Popelka, FIT
// Přeloženo: gcc 7.5
// Soubor obsahuje implementaci funkce Eratosthenes, která využívá Eratosthenova síta, pro nalezení prvočísel v bitovém poli typu bitset_t
// Popis a prototyp funkce eratosthenes.h jsou v souboru eratosthenes.h
// Využívá datových typů a funkcí z hlavičkového souboru bitset.h, který pracuje nad bitovými poli

#include "eratosthenes.h"
void Eratosthenes(bitset_t pole)
{   
    /* Nastavení čísla 0 a 1 jako čísla, která nejsou prvočísla*/
    bitset_setbit(pole,0UL,1);
    bitset_setbit(pole,1UL,1);

    for(unsigned long i = 2; i < sqrt(pole[0]); i++)
    {
        if(!(bitset_getbit(pole,i))) //Pokud je prvočíslo
        {
            for(unsigned long j = i*2; j <= pole[0]; j+=i)  // Nastavení násobků čísla jako prvočísla
            {
                  bitset_setbit(pole,j,1); 
            }
        }
    }
}