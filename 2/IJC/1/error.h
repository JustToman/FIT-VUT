// error.c
// Řešení IJC-DU1, příklad b), 12.3.2020
// Autor: Roman Popelka, FIT
// Přeloženo: gcc 7.5
// Umožňuje vypisovat zprávy o chybách
// Popisy a definice funkcí se nachází v souboru error.c
// Využívá knihoven stdio.h stdlib.h stdarg.h
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>


void warning_msg(const char *fmt, ...);
void error_exit(const char *fmt, ...);
