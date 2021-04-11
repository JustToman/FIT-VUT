// htab.h
// Řešení IJC-DU2, příklad  b) 15.4.2020
// Autor: Roman Popelka, FIT
// Přeloženo: gcc 7.5

// htab.h -- rozhraní knihovny htab (řešení IJC-DU2)
// Licence: žádná (Public domain)
// následující řádky zabrání násobnému vložení:
#ifndef __HTAB_H__
#define __HTAB_H__
#include <string.h>
#include <stdbool.h> 
#include <stdint.h>
#include <stdlib.h>
#include "htab_item.h"
#include "htab_t.h" 
 
#define MAX_WORD_LEN 128
// Tabulka:
struct htab;     // neúplná deklarace struktury - uživatel nevidí obsah
typedef struct htab htab_t;  // typedef podle zadání

// Typy:
typedef const char * htab_key_t; // typ klíče
typedef int htab_value_t;  // typ hodnoty

// Iterátor do tabulky:
struct htab_item; // neúplná deklarace struktury 

// iterátor:
typedef struct htab_iterator {
struct htab_item *ptr; //ukazatel na položku
const htab_t *t; //ve které tabulce
size_t idx; //ve kterém seznamu tabulky (TODO: optimalizovat?)
} htab_iterator_t;  // typedef podle zadání

// rozptylovací (hash) funkce (stejná pro všechny tabulky v programu)
// pokud si v programu definujete stejnou funkci, použije se ta vaše
size_t htab_hash_fun(htab_key_t str);

// funkce pro práci s tabulkou:
htab_t *htab_init(size_t n); // konstruktor tabulky
size_t htab_size(const htab_t * t); // počet záznamů v tabulce
size_t htab_bucket_count(const htab_t * t); // velikost pole


htab_iterator_t htab_find(htab_t * t, htab_key_t key); // hledání
htab_iterator_t htab_lookup_add(htab_t * t, htab_key_t key);

void htab_erase(htab_t * t, htab_iterator_t it);// ruší zadaný záznam
htab_iterator_t htab_begin(const htab_t * t);// iterátor na první záznam
htab_iterator_t htab_end(const htab_t * t);// iterátor _za_ poslední záznam



htab_iterator_t htab_iterator_next(htab_iterator_t it); // iterátor++

// test: iterátor != end()
static inline bool htab_iterator_valid(htab_iterator_t it) { return it.ptr!=NULL; }
// test: iterátor1 == iterátor2 
static inline bool htab_iterator_equal(htab_iterator_t it1, htab_iterator_t it2) 
{
return it1.ptr==it2.ptr && it1.t == it2.t && it1.idx == it2.idx;              //upravil jsem htab_iterator_equal pridanim it1.idx == it2.idx 
}

// čtení a zápis přes iterátor
htab_key_t htab_iterator_get_key(htab_iterator_t it);
htab_value_t htab_iterator_get_value(htab_iterator_t it);
htab_value_t htab_iterator_set_value(htab_iterator_t it, htab_value_t val);
void htab_clear(htab_t * t); // ruší všechny záznamy
void htab_free(htab_t * t); // destruktor tabulky

#endif // __HTAB_H__