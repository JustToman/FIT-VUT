// io.c
// Řešení IJC-DU2, příklad  b) 15.4.2020
// Autor: Roman Popelka, FIT
// Přeloženo: gcc 7.5
#include "io.h"


int get_word(char *s, int max, FILE *f)
{
    int word_len = 0;

    while((s[word_len] = fgetc(f)) != EOF)
    {
        if(isspace(s[word_len])) // if character equals to space character breaks
            break;
            
        word_len++;
        
        if(word_len == max)
        {
            break;
        }
    }
    if(s[word_len] == EOF)
        return EOF;
    
    s[(word_len == max) ? word_len - 1 : word_len] = '\0'; // sets zero char
    
    return word_len;
}
