// primes.c
// Řešení IJC-DU1, příklad a), 12.3.2020
// Autor: Roman Popelka, FIT
// Přeloženo: gcc 7.5
// Slouží pro nalezení posledních 10 prvočísel z 500000000, kde jsou nalezená čísla řazena vzestupně
// Využívá implementaci eratosthenova síta z knihovny eratosthenes.h a bitového pole z knihovny bitset.h
#include "eratosthenes.h"
#include  <stdio.h>
#include  <time.h>


int main()
{
    bitset_create(pole,500000000); // vytvoření pole
    clock_t start = clock(); // zaznamenání starovního času
    unsigned long last_primes[10]; // pole pro posledních 10 prvočísel
    int n = 0; // čítač

  
   Eratosthenes(pole); // zjišťování prvočísel
  
   /* Cyklus, který zapíše posledních 10 prvočísel*/
   for(unsigned long i=500000000;i>1;i--)
    {
        if(!bitset_getbit(pole,i)) // Pokud je prvočíslo, zapíše se index do pole last_primes a zvýší se hodnota čítače
        {
          last_primes[n] = i;
          n++;
        }
        if(n == 10) // pokud je n=10, vyskočí z cyklu
        {
          break;
        }
    }
    /* vypsání posledních 10 prvočísel vzestupně  */
    for(long i = 9; i>=0; i--)
    {
      printf("%ld\n",last_primes[i]);
    }

    fprintf(stderr, "Time=%.3g\n", (double)(clock()-start)/CLOCKS_PER_SEC); //Vypíše čas

    
}
