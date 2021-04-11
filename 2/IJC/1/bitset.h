// bitset.h
// Řešení IJC-DU1, příklad a), 12.3.2020
// Autor: Roman Popelka, FIT
// Přeloženo: gcc 7.5
// Vytváří hlavičkový soubor sloužící pro práci s jednotlivými bity nad definovaným datovým typem bitset_t
// Pole je možné vytvořit staticky a dynamicky na stacku i alokovat na heapu
// Při definování USE_INLINE při překladu, použije inline funkce, jinak používá makra.
// Prototypy inline funkcí se nacházejí v souboru bitset.c
// Využívá funkcí z hlavičkového souboru error.h, stdlib.h a assert.h

#include <assert.h>
#include <stdlib.h>
#include "error.h"
#include <math.h>

/*Bitové pole typedef*/
typedef unsigned long *bitset_t;      
/*Index do bitového pole*/      
typedef unsigned long bitset_index_t;

#define BITS_IN_BYTE __CHAR_BIT__   // Definuje počet bitů v bytu
#define SIZE sizeof sizeof(unsigned long) * BITS_IN_BYTE // Velikost unsigned longu v bitech


/**
 * @brief Vyvoří bitové pole o jméně "jmeno_pole" typu bitset_t o velikosti "velikost" na stacku
 * Pokud je zadána velikost pole menší než 1, program zahlásí chybu při překladu.
 * Do 0. indexu pole se uloží velikost pole v bitech
 */

#define bitset_create(jmeno_pole, velikost) static_assert((velikost) > 1,"Chyba pri prekladu");  unsigned long jmeno_pole[((velikost) / (SIZE))  + (((velikost) % (SIZE)) ? 1 : 0) + 2] = {(velikost)}  

/**
 * @brief Vyvoří bitové pole o jméně "jmeno_pole" typu bitset_t o velikosti "velikost" na heapu
 * Pokud je zadána velikost pole menší než 1, program se ukončí
 * Pokud je po alokaci pole rovno NULL, program se ukončí s chybovou hláškou: "bitset_alloc: Chyba alokace paměti"
 * Do 0. indexu pole se uloží velikost pole v bitech
 */

#define bitset_alloc(jmeno_pole, velikost)   unsigned long *jmeno_pole = calloc((velikost) / (sizeof(unsigned long) * BITS_IN_BYTE)\
                                             + (((velikost) % (sizeof(unsigned long) * BITS_IN_BYTE)) > 0 ? 1 : 0) + 1, sizeof(unsigned long));\
                                            assert(velikost > 1); if(jmeno_pole == NULL){fprintf(stderr,"bitset_alloc: Chyba alokace paměti\n"); exit(1);}\
                                            jmeno_pole[0] = (velikost)

/* Pokud není definováno použití Inline funkcí*/
#if !defined(USE_INLINE)

  /**
   * @brief Uvolní paměť alokovaného pole o jméně "jmeno_pole"
   * 
   */
  #define bitset_free(jmeno_pole)  free(jmeno_pole)
  /**
   * @brief Vrátí velikost pole v bitech o jméně "jmeno_pole"
   * 
   */
  #define bitset_size(jmeno_pole)  jmeno_pole[0]
  /**
   * @brief Vrací hodnotu bitu v poli o jméně "jmeno_pole" typu bitset_t na indexu "index"
   * 
   */
  #define bitset_getbit(jmeno_pole, index) ((index > (bitset_size(jmeno_pole))) ? ((error_exit("bitset_getbit: Index %lu mimo rozsah 0..%lu",(unsigned long)index, (unsigned long)bitset_size(jmeno_pole))),EXIT_FAILURE) : (((jmeno_pole[(index/(SIZE)) + 1]) & (1UL << (index % (SIZE)))) ? 1:0))
  /**
   * @brief Nastaví hodnotu bitu v poli typu bitset_t o jméně "jmeno_pole" na indexu "index" na hodnotu výrazu "vyraz"
   * 
   */
  #define bitset_setbit(jmeno_pole, index, vyraz) if(index > (bitset_size(jmeno_pole))) error_exit("bitset_getbit: Index %lu mimo rozsah 0..%lu",(unsigned long)index, (unsigned long)bitset_size(jmeno_pole));\
                                                  if((vyraz)) jmeno_pole[(index / (SIZE)) + 1] |= (1UL<<(index % (SIZE)));\
                                                  if(!(vyraz)) jmeno_pole[(index/ (SIZE)) + 1]  &= ~(1UL<<(index % (SIZE)))

/* Pokud je definováno použití inline funkcí*/
#else
  /**
   * @brief Uvolní paměť pole
   * 
   * @param jmeno_pole pole typu bitset_t jehož paměť se uvolňuje
   */
  inline void bitset_free(bitset_t jmeno_pole)
  {
        free(jmeno_pole);
  }
  /**
   * @brief Vrátí velikost pole v bitech
   * 
   * @param jmeno_pole pole typu bitset_t jehož velikost se vrací
   * @return unsigned long velikost pole v bitech
   */
  inline unsigned long bitset_size(bitset_t jmeno_pole)
  {
    return jmeno_pole[0];
  }
  /**
   * @brief Vrací hodnotu bitu na zadaném indexu
   * Pokud je hodnota indexu mimo meze tzn. 0 - bitset_size(jmeno_pole) volá se funkce error_exit
   * 
   * @param jmeno_pole pole typu bitset_t
   * @param index index bitu, jehož hodnotu se zjišťuje
   * @return int hodnota bitu 
   */
  inline int bitset_getbit(bitset_t jmeno_pole, bitset_index_t index)
  {
    if(index > bitset_size(jmeno_pole))
      error_exit("bitset_getbit: Index %lu mimo rozsah 0..%lu",(unsigned long)index, (unsigned long)bitset_size(jmeno_pole));
    return (jmeno_pole[(index/(SIZE)) + 1] & (1UL << (index % (SIZE))))? 1: 0;
  }
  /**
   * @brief Nastavuje hodnotu bitu na zadaném indexu na hodnotu vyraz
   * Pokud je hodnota indexu mimo meze tzn. 0 - bitset_size(jmeno_pole) volá se funkce error_exit
   * 
   * @param jmeno_pole ukazatel na pole typu bitset_t
   * @param index index bitu, jehož hodnota se mění
   * @param vyraz výraz na jehož hodnotu se bit mění 
   */
  inline void bitset_setbit(bitset_t jmeno_pole, bitset_index_t index, unsigned long vyraz)
  {
    if(index > bitset_size(jmeno_pole))
      error_exit("bitset_getbit: Index %lu mimo rozsah 0..%lu",(unsigned long)index, (unsigned long)bitset_size(jmeno_pole));
    if(vyraz)
      jmeno_pole[index/(SIZE) + 1] |= (1UL << (index % (SIZE)));
    if(!vyraz)
      jmeno_pole[index/(SIZE) + 1] &= (1UL << (index % (SIZE)));
  }
#endif


                                  
