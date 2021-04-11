// wordcount.c
// Řešení IJC-DU2, příklad  b) 15.3.2020
// Autor: Roman Popelka, FIT
// Přeloženo: gcc 7.5
#include <stdio.h>
#include "htab.h"
#include "io.h"

#define MAX_WORD_LEN 128
#define LIMIT_EXCEEDED_ERROR 2;

int main()
{
    char word[MAX_WORD_LEN];         // string storage
    bool line_overflow = 0; // dectecting if line length exceeded maximal word length
    int word_len = 0;       // string length
    htab_t * t = htab_init(10000);  // hash table constructor
    htab_iterator_t it = htab_begin(t); // setting iterator to beginning
    int exit_code = 0;

    /* Loading strings to hashtable */
    while((word_len = get_word(word,MAX_WORD_LEN,stdin)) !=  EOF)
    {
         /* Checking if word length exceeded limit. If it did, prints warning on stderr for first time this happened */
        if(word_len >= 128 && line_overflow == false)
        {
            fprintf(stderr,"Length of word exceeded 127 letters\n");
            line_overflow = true;
            exit_code = LIMIT_EXCEEDED_ERROR;
        }
        
        if(word[0] == '\0') // Skipping empty array
            continue;

        it = htab_lookup_add(t,word); // Word finding/adding
        if(!htab_iterator_valid(it))
            return EXIT_FAILURE;

        if(htab_iterator_set_value(it,htab_iterator_get_value(it) + 1) == -1)          // Increment number of word appearance
            return EXIT_FAILURE;

        
    }
   
    /* Looping through hash_table and printing all keys and number of occurences*/
    for(it = htab_begin(t); !htab_iterator_equal(it,htab_end(t)); it = htab_iterator_next(it))
    {
        if(it.ptr != NULL)
            printf("%s %d\n",htab_iterator_get_key(it), htab_iterator_get_value(it));
    }
    htab_free(t); // Frees hash table
   
    return exit_code;
}




