//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Test Driven Development - priority queue code
//
// $NoKeywords: $ivs_project_1 $tdd_code.cpp
// $Author:     JMENO PRIJMENI <xlogin00@stud.fit.vutbr.cz>
// $Date:       $2017-01-04
//============================================================================//
/**
 * @file tdd_code.cpp
 * @author JMENO PRIJMENI
 * 
 * @brief Implementace metod tridy prioritni fronty.
 */

#include <stdlib.h>
#include <stdio.h>

#include "tdd_code.h"

//============================================================================//
// ** ZDE DOPLNTE IMPLEMENTACI **
//
// Zde doplnte implementaci verejneho rozhrani prioritni fronty (Priority Queue)
// 1. Verejne rozhrani fronty specifikovane v: tdd_code.h (sekce "public:")
//    - Konstruktor (PriorityQueue()), Destruktor (~PriorityQueue())
//    - Metody Insert/Remove/Find a GetHead
//    - Pripadne vase metody definovane v tdd_code.h (sekce "protected:")
//
// Cilem je dosahnout plne funkcni implementace prioritni fronty implementovane
// pomoci tzv. "double-linked list", ktera bude splnovat dodane testy 
// (tdd_tests.cpp).
//============================================================================//

PriorityQueue::PriorityQueue()
{
  root = new Root;
  root->pHead = NULL;
}

PriorityQueue::~PriorityQueue()
{
  Element_t *tmp, *tmp2;
  if(root->pHead != NULL) {
    for(tmp = root->pHead; tmp->pNext != NULL; tmp = tmp->pNext)
    {
      if(tmp->pPrev != NULL)
      {
        tmp2 = tmp->pPrev;
        delete tmp2; 
      }
    } 
    delete tmp;
  }
  delete root;
}

void PriorityQueue::Insert(int value)
{
  Element_t * new_element = new Element_t;
  new_element->value = value;

  if(root->pHead != NULL)
  {
    Element_t *tmp = root->pHead;
    for(tmp; tmp != NULL; tmp = tmp->pNext)
    {
      if(tmp->value >= value) {
        new_element->pNext = tmp;
        new_element->pPrev = tmp->pPrev;
        if(tmp->pPrev != NULL)
          tmp->pPrev->pNext = new_element;
        tmp->pPrev = new_element;
        if(tmp == root->pHead)
          root->pHead = new_element;
        break;
      }
      else if(tmp->value < value && tmp->pNext == NULL) {
        new_element->pPrev = tmp;
        new_element->pNext = NULL;
        tmp->pNext = new_element;
        break;
      }
    }
  }
  else {
    root->pHead = new_element;
    new_element->pNext = NULL;
    new_element->pPrev = NULL;
  }
}

bool PriorityQueue::Remove(int value)
{
  Element_t *tmp;
  if(root->pHead != NULL) {
    for(tmp = root->pHead; tmp != NULL; tmp = tmp->pNext) {
     if(tmp->value == value){
      if(tmp == root->pHead){
        if(tmp->pNext != NULL){
          root->pHead = tmp->pNext;
          tmp->pNext->pPrev = NULL;
        }
        else
          root->pHead = NULL;
      }
      else if(tmp->pPrev != NULL){
        if(tmp->pNext != NULL){
         tmp->pPrev->pNext = tmp->pNext;
         tmp->pNext->pPrev = tmp->pPrev;
        }
        else
         tmp->pPrev->pNext = NULL;
      }
      else
       tmp->pNext->pPrev = NULL;
      delete tmp;
      return true;
    }
   }
  }
  return false;
}

PriorityQueue::Element_t *PriorityQueue::Find(int value)
{  
  if(root->pHead != NULL)
  {
    Element_t *tmp;
    for(tmp = root->pHead; tmp != NULL; tmp = tmp->pNext){
      if(value == tmp->value)
      {
        return tmp;
      }
    }
  }
  return NULL;
}

PriorityQueue::Element_t *PriorityQueue::GetHead()
{
  if(root->pHead != NULL){
      return root->pHead;
    }
  else
      return NULL;
}

/*** Konec souboru tdd_code.cpp ***/
