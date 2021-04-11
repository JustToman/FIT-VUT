// htab_iterator_next.c
// Řešení IJC-DU2, příklad  b) 15.4.2020
// Autor: Roman Popelka, FIT
// Přeloženo: gcc 7.5
#include "htab.h"


/**
 * @brief Returns iterator to next element, if next element isn't found returns htab_end
 * 
 * 
 * @param it iterator
 * @return htab_iterator_t 
 */
htab_iterator_t htab_iterator_next(htab_iterator_t it)
{
    htab_iterator_t end_iterator = htab_end(it.t);
    htab_iterator_t tmp = it;

   /* While iterator != htab_end()*/
   while(it.idx != end_iterator.idx)
   {
       if(tmp.ptr != NULL && tmp.ptr != it.ptr)   // is this next element? if yes return it
        return tmp;

       if(tmp.ptr == NULL || tmp.ptr->next == NULL) // is this end of list?
       {
           tmp.idx++; //idx ++
           if(tmp.idx == end_iterator.idx) // end test
            break;
           tmp.ptr = it.t->array[tmp.idx]; // next array element
       } 
       else
       {
           tmp.ptr = tmp.ptr->next; 
       }
   }
     tmp = htab_end(it.t); //Setting end
    return tmp;
}