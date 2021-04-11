// io.h
// Řešení IJC-DU2, příklad  b) 15.4.2020
// Autor: Roman Popelka, FIT
// Přeloženo: gcc 7.5
#include <ctype.h>
#include <stdio.h>

/**
 * @brief Stores string to passed parameter s
 * 
 * @param s pointer to string 
 * @param max max string length
 * @param f file from which is string read
 * @return int number of chars in string
 */
int get_word(char *s, int max, FILE *f);
