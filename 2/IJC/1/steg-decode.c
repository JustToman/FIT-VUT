// steg-decode.c
// Řešení IJC-DU1, příklad b), 12.3.2020
// Autor: Roman Popelka, FIT
// Přeloženo: gcc 7.5
// Slouží pro nalezení zakódované zprávy v datech načtených z obrázku formátu .ppm
// Pro načtení využívá knihovny ppm.h, pro práci s bity využívá knihovny bitset.h, pro hledání prvočísel využívá implementace eratosthenova
// síta s knihovny eratosthenes.h
// Prototypy a komentáře funkcí jsou v souboru steg-decode.h
#include "steg-decode.h"

int main(int argc, char *argv[])
{

    /* Kontrola argumentu */
    if(argc != 1)  
      error_exit("steg-decode: Špatně zadané argumenty");
 
    /* Načtení obrázku, alokace a jejich kontroly */
    struct ppm *ppm = ppm_read(argv[1]); 
    if(ppm == NULL)
        error_exit("steg-decode - ppm read bylo neúspěšné");
    bitset_alloc(primes,(ppm->xsize * ppm->ysize *3));  // alokování bitového pole o stejné velikosti
    if(primes == NULL)
        error_exit("steg-decode - bitset_alloc neproběhlo úspěšně");

    /*Hledání prvočísel*/
    Eratosthenes(primes);
    
    /*Práce se zprávou*/
    unsigned long n_of_primes = get_n_of_primes(primes);  // Určování počtu prvočísel 
    const unsigned long msg_length = (n_of_primes / (BITS_IN_BYTE * sizeof(unsigned char))) + ((n_of_primes % (BITS_IN_BYTE * sizeof(unsigned char)) ? 1:0)); // nastavení maximální délky "tajné" zprávy
    unsigned char message[msg_length]; // vytvoření pole znaků pro uložení zprávy 
    int is_null_char = 0;    

    /* Nulování zprávy */
    for(unsigned long i = 0; i < msg_length; i++)
        message[i] = 0;

    get_message(message,ppm,primes);

    /* Hledání nulového znaku */
    for(unsigned int i = 0; i < msg_length; i++)
    {
        if(message[i] == '\0')
        {
            is_null_char = 1;
        }
    }
    if(!is_null_char)
        error_exit("steg-decode: zpráva neobsahuje nulový znak");

  
    /* Výpis zprávy*/
    printf("%s\n",message);

    /*Uvolnění místa*/
    bitset_free(primes);
    ppm_free(ppm);
    
    return EXIT_SUCCESS;
}

unsigned long get_n_of_primes(const bitset_t pole)
{
    unsigned long n = 0;
    for(unsigned long i=0; i<bitset_size(pole);i++)
    {
        if(!(bitset_getbit(pole,i)))  // Je prvočíslo?
            n++;                    // Inkrementace čítače
    }
    return n;
}

void get_message(unsigned char *message, const struct ppm *ppm,const bitset_t pole)
{
    
   unsigned long j = 0; // message bit index
   for(unsigned long i = 23; i < bitset_size(pole); i++)
   {
       if(!bitset_getbit(pole,i))        // Je prvočíslo?
       {
         if((char_getbit(ppm->data,i)))
            message[j/(sizeof(char)*(SIZE_CHAR))] |= 1 << (j%(sizeof(char)*(SIZE_CHAR)));
         else if(!(char_getbit(ppm->data,i)))
            message[j/(sizeof(char)*(SIZE_CHAR))] &= ~(1 << (j%(sizeof(char)*(SIZE_CHAR))));
        j++;
       }
   } 
}
