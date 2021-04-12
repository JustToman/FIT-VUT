
/* c201.c *********************************************************************}
{* Téma: Jednosměrný lineární seznam
**
**                     Návrh a referenční implementace: Petr Přikryl, říjen 1994
**                                          Úpravy: Andrea Němcová listopad 1996
**                                                   Petr Přikryl, listopad 1997
**                                Přepracované zadání: Petr Přikryl, březen 1998
**                                  Přepis do jazyka C: Martin Tuček, říjen 2004
**                                              Úpravy: Kamil Jeřábek, září 2020
**
** Implementujte abstraktní datový typ jednosměrný lineární seznam.
** Užitečným obsahem prvku seznamu je celé číslo typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou typu tList.
** Definici konstant a typů naleznete v hlavičkovém souboru c201.h.
**
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ tList:
**
**      DisposeList ... zrušení všech prvků seznamu,
**      InitList ...... inicializace seznamu před prvním použitím,
**      InsertFirst ... vložení prvku na začátek seznamu,
**      First ......... nastavení aktivity na první prvek,
**      CopyFirst ..... vrací hodnotu prvního prvku,
**      DeleteFirst ... zruší první prvek seznamu,
**      PostDelete .... ruší prvek za aktivním prvkem,
**      PostInsert .... vloží nový prvek za aktivní prvek seznamu,
**      Copy .......... vrací hodnotu aktivního prvku,
**      Actualize ..... přepíše obsah aktivního prvku novou hodnotou,
**      Succ .......... posune aktivitu na další prvek seznamu,
**      Active ........ zjišťuje aktivitu seznamu.
**
** Při implementaci funkcí nevolejte žádnou z funkcí implementovaných v rámci
** tohoto příkladu, není-li u dané funkce explicitně uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam předá
** někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

#include "c201.h"

int errflg;
int solved;

void Error() {
/*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;                      /* globální proměnná -- příznak chyby */
}

void InitList (tList *L) {
/*
** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádět nad již inicializovaným
** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
** že neinicializované proměnné mají nedefinovanou hodnotu.
**/

 L->Act = NULL;
 L->First = NULL;
}

void DisposeList (tList *L) {
/*
** Zruší všechny prvky seznamu L a uvede seznam L do stavu, v jakém se nacházel
** po inicializaci. Veškerá paměť používaná prvky seznamu L bude korektně
** uvolněna voláním operace free.
***/
    
    //temp variable for looping through list	
    tElemPtr element= L->First;

    /*Loop where we create temp. variable for next_element, assign next element of element to  next_element and free element, then we assign next_element to element and continue until element == NULL*/
    while(element != NULL)
    {
        tElemPtr next_element  = element->ptr;
        free(element);
        element = next_element;
    }
    /* Setting list variables to values after initialization*/
    L->Act = NULL;
    L->First = NULL;
}

void InsertFirst (tList *L, int val) {
/*
** Vloží prvek s hodnotou val na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci Error().
**/
 
 //Allocating new element, if allocation wasn't successful prints Error msg and returns   
 tElemPtr new_element = (tElemPtr) malloc(sizeof(struct tElem));
 if (new_element == NULL)
 {
    Error();
    return;
 }
 

 new_element->data = val;
 new_element->ptr = L->First;
 L->First = new_element;
}

void First (tList *L) {
/*
** Nastaví aktivitu seznamu L na jeho první prvek.
** Funkci implementujte jako jediný příkaz, aniž byste testovali,
** zda je seznam L prázdný.
**/
    //Sets activity to First element	
    L->Act = L->First;
}

void CopyFirst (tList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci Error().
**/
    //If list isn't empty returns First element's data to variable val. If it's empty prints err msg and returns
    if(L->First != NULL)
	    *val = L->First->data;
    else
    {
        Error();
        return;
    }
    
}

void DeleteFirst (tList *L) {
/*
** Zruší první prvek seznamu L a uvolní jím používanou paměť.
** Pokud byl rušený prvek aktivní, aktivita seznamu se ztrácí.
** Pokud byl seznam L prázdný, nic se neděje.
**/
    //If list isn't empty
    if(L->First != NULL)
    {
	//create new first element (that's next element of first element)
    	tElemPtr new_first = L->First->ptr;
	//if activity is on first element, delete activity
        if(L->Act == L->First)
            L->Act = NULL;
	//free first element
        free(L->First);
	//set new_first element as first element in list
        L->First = new_first;
    }
}	

void PostDelete (tList *L) {
/* 
** Zruší prvek seznamu L za aktivním prvkem a uvolní jím používanou paměť.
** Pokud není seznam L aktivní nebo pokud je aktivní poslední prvek seznamu L,
** nic se neděje.
**/
	if(L->Act != NULL && L->Act->ptr != NULL)
    {
            tElemPtr deleted_next = L->Act->ptr->ptr;
            free(L->Act->ptr);
            L->Act->ptr = deleted_next; 
    }
	
}

void PostInsert (tList *L, int val) {
/*
** Vloží prvek s hodnotou val za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje!
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** zavolá funkci Error().
**/
    // If list has active element set
    if(L->Act != NULL)
    {
	//Allocate new element
        tElemPtr new_element = (tElemPtr) malloc(sizeof(struct tElem));
      
	//Test new element allocation, if allocation was unsuccessful, print err msg and return
        if(new_element == NULL) 
        {
            Error();
            return;
        }

	//new_element data assignment
        new_element->data = val;
	//Connecting elements pointers
        new_element->ptr = L->Act->ptr;
        L->Act->ptr = new_element;
    }
}

void Copy (tList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam není aktivní, zavolá funkci Error().
**/
    //if list doesn't have active element, prints err msg and returns	
    if(L->Act == NULL)
    {
        Error();
        return;
    }
    //otherwise copies Active element's data to "val"
    *val = L->Act->data;
}

void Actualize (tList *L, int val) {
/*
** Přepíše data aktivního prvku seznamu L hodnotou val.
** Pokud seznam L není aktivní, nedělá nic!
**/
	//If list does have active element, rewrites it's data to "val"
	if(L->Act != NULL)
          L->Act->data = val;
}

void Succ (tList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Všimněte si, že touto operací se může aktivní seznam stát neaktivním.
** Pokud není předaný seznam L aktivní, nedělá funkce nic.
**/

    //If list does have active element, sets active element to Active element's next element
    if(Active(L))
        L->Act = L->Act->ptr;
}

int Active (tList *L) {
/*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Tuto funkci je vhodné implementovat jedním příkazem return. 
**/
	//Returns if List has active element or not (0 or 1)
	return L->Act != NULL;
}

/* Konec c201.c */
