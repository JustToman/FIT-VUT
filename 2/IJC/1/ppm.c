// ppm.c
// Řešení IJC-DU1, příklad b), 12.3.2020
// Autor: Roman Popelka, FIT
// Přeloženo: gcc 7.5
// Knihovna určená pro načtení dat ze souboru .ppm
// Omezení: Načtení obrázku o maximální velikosti 8000 * 8000 pixelů
// Prototypy funkcí v ppm.h
#include "ppm.h"


/**
 * @brief Načte data ze souboru do struktury alokované na heapu a vrátí na ni ukazatel
 * 
 * @param filename jméno souboru ze které se čtou data
 * @return struct ppm* ukazatel na strukturu
 */
struct ppm * ppm_read(const char *filename)
{
    struct ppm *tmp = malloc(sizeof(struct ppm)); //Alokace struktury
    FILE *file; 
    file = fopen(filename,"r"); // otevření souboru
    
    if(!file) // Ověření otevření souboru
        error_exit("open_file, soubor %s se nepodařilo otevřít\n",filename); 
    
    if(!check_and_load_file_info(file,tmp)) //Ověření formátu souboru
    {
        warning_msg("Špatný formát\n");
        ppm_free(tmp);
        fclose(file);
        return NULL;
    }

    if((tmp->data = malloc(tmp->xsize * tmp->ysize * 3)) == NULL) // Alokování místa pro data
    {
        warning_msg("Nepodařilo se alokovat místo pro data\n");
        ppm_free(tmp);
        fclose(file);
        return NULL;
    }


    if(fread(tmp->data,sizeof(unsigned char),tmp->xsize * tmp->ysize * 3,file) != (tmp->ysize * tmp->xsize * 3))  // Čtení dat po bytu do tmp->data
    {
        warning_msg("Nepodarilo se precist vsechna data\n");
        ppm_free(tmp);
        fclose(file);
        return NULL;
    } 

    fclose(file);
    return tmp;
}

/**
 * @brief Uvolní alokovanou strukturu
 * 
 * @param ppm struktura, která se uvolňuje z paměti
 */
void ppm_free(struct ppm *ppm)
{
    free(ppm->data);
    free(ppm);
}

/**
 * @brief Zkontroluje formát souboru a načte data o velikosti do struktury
 * 
 * @param file odkaz na soubor ze které se čte
 * @param ppm struktura do které se načítá
 * @return int zpráva o úspěchu operace 0-neúspěch 1-úspěch
 */
int check_and_load_file_info(FILE *file, struct ppm *ppm)
{
    char buffer[BUFFER_SIZE];
    if(fscanf(file,"%99s",buffer) != 1 && strcmp(buffer,"P6"))
        return 0;
    if((fscanf(file,"%u %u",&ppm->xsize, &ppm->ysize)) != 2)  //Scanování velikosti obrázku
        return 0;
    if((ppm->xsize <=0 || ppm->ysize <=0) || (ppm->xsize > MAX_X_SIZE || ppm->ysize > MAX_Y_SIZE)) // Kontrola velikosti obrázku
        return 0;
    
    if(fscanf(file,"%99s",buffer) != 1 && strcmp("255",buffer))
        return 0; 

    if(fgets(buffer,BUFFER_SIZE-1,file) == NULL) // Odřádkování
        return 0;
    return 1;
}


