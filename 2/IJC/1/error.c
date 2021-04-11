// error.c
// Řešení IJC-DU1, příklad b), 12.3.2020
// Autor: Roman Popelka, FIT
// Přeloženo: gcc 7.5
// Umožňuje vypisovat zprávy o chybách
// Prototypy funkcí se nacházejí v souboru error.h
#include "error.h"


/**
 * @brief Vypíše varování
 * 
 * @param fmt formát zprávy
 * @param ... argumenty
 */
void warning_msg(const char *fmt, ...)
{
    va_list args;
    va_start(args,fmt);
    fprintf(stderr,"\nCHYBA: ");
    vfprintf(stderr,fmt,args);
    va_end(args);
}
/**
 * @brief Vypíše chybovou hlášku a ukončí program
 * 
 * @param fmt formát zprávy
 * @param ... argumenty
 */
void error_exit(const char *fmt, ...)
{
    va_list args;
    va_start(args,fmt);
    fprintf(stderr,"\nCHYBA: ");
    vfprintf(stderr,fmt,args);
    va_end(args);
    exit(EXIT_FAILURE);
}
