#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/* MACROS DEFINITIOS */
#define LEFT_BORDER 1
#define RIGHT_BORDER 2
#define FLAT_BORDER 4
#define LEFT_HAND 6
#define RIGHT_HAND 9

#define ERR(str) (fprintf(stderr,"%s",(str)))

/* Struct */
typedef struct {
    int rows;
    int cols;
    unsigned char *cells;
} Map;

/*FUNCTION PROTOTYPES*/
int check_map_validity(char *filename);
int check_format(FILE *file);
int check_first_line(FILE *file);
int check_maze_size(FILE *file, int rows, int cols);
int map_ctor(Map *map, FILE *filename);
bool is_border(Map *map, int r, int c, int border);
bool check_borders(Map *map);
int start_border(Map *map, int row, int col);
void find_right_path(Map *map,int r, int c);
bool is_num(char *str);
bool check_args(int argc, char *argv[],Map *map);
void print_help();
void find_left_path(Map *map, int r, int c);


int main(int argc, char *argv[])
{
    Map map;
    FILE *fr;
   /* Arguments check*/
    if (check_args(argc, argv, &map)) {
      if (argc == 5) { 
       /* ERROR CHECK */
        if(!check_map_validity(argv[4])) {
          ERR("MAP NOT VALID\n");
          return 1;
        }
        if (!(fr = fopen(argv[4], "r"))) {
          ERR("File cannot be opened\n");
          return 1;
        }
        if(!map_ctor(&map, fr)){
          ERR("MAP CANT BE CREATED\n");
          return 1;
        }
        if(!check_borders(&map)){
          ERR("MAP NOT VALID\n");
          free(map.cells);
          fclose(fr);
          return 1;
        }


        /* Map constructing*/
        int rows = atoi(argv[2]);
        int cols = atoi(argv[3]);

        /*Checking entered rows and cols*/
        if (rows > map.rows) {
          ERR("Wrong start coordinates\n");
          fclose(fr);
          free(map.cells);
          return 1;
        }
        if (cols > map.cols) {
          fclose(fr);
          free(map.cells);
          ERR("Wrong start coordinates\n");
          return 1;
        }
        if (!start_border(&map, rows, cols)) {
          ERR("Cannot acces maze here\n");
          fclose(fr);
          free(map.cells);
          return 1;
        }

       /* leftright choice */
        if (!strcmp(argv[1], "--lpath"))
          find_left_path(&map, rows, cols);
        if (!strcmp(argv[1], "--rpath"))
          find_right_path(&map, rows, cols);
        
        /* Close file and frees map.cells*/
        fclose(fr);
        free(map.cells);  
        }
      }

    return 0;
}

/**
 * @brief Checks if map is valid
 * 
 * @param filename File name
 * @return int 
 */
int check_map_validity(char *filename)
{
  int rows, cols;

  FILE* fr = fopen(filename, "r");
  if (!fr) {
      ERR("Error: File can't be opened\n");
      fclose(fr);
      return 0;
  }
  if(!check_first_line(fr) || !check_format(fr))
    return 0;

  fscanf(fr, "%d %d", &rows, &cols);

  if (!check_maze_size(fr, rows, cols))
    return 0;

  fclose(fr);
  return 1;
}

/**
 * @brief Checks if maze format is entered correctly
 * 
 * @param file pointer to txt file containing maze
 * @return int int Returns value depending on success of control, if it was sucessful 1, otherwise 0
 */
int check_format(FILE *file)
{
  // Character
    int c;
  // Counter
    int n=0;
  // Skips 1st row
    while ((c = fgetc(file)) != '\n');
  // Checks if data equals specified format
    while ((c = fgetc(file)) != EOF) {
      if (c != '\n') {
        if ((n % 2) == 0 && !isdigit(c))
          return 0;
        else if ((n%2) == 1 && c != ' ' )
          return 0;
        if (isdigit(c) && c > '7')
          return 0;
      }
      n++;
    }
    rewind(file);
    return 1;
}

/**
 * @brief Checks if first line contains valid data
 * 
 * @param file pointer to txt file containing maze
 * @return int Returns value depending on success of control, if it was sucessful 1, otherwise 0 
 */
int check_first_line(FILE *file)
{
   /* Number of numbers in files initialization row*/
    int numbers = 0;  
    int c = fgetc(file);
    /* Previous character check*/
    int cprev = c;
    while ((c = fgetc(file)) != '\n' && c != EOF) {
      if (isdigit(c) || c == ' ') {
        if (isdigit(cprev) && c == ' ' ) { 
          numbers++;
          if (numbers > 2)
            return 0;
        }
        cprev = c;
      }
      else
        return 0;
    }
    rewind(file);
    return 1;
}

/**
 * @brief Checks maze size
 * 
 * @param file pointer to txt file containing maze
 * @param rows Number of rows of maze
 * @param cols Number of colomns of maze
 * @return int Returns value depending on success of control, if it was sucessful 1, otherwise 0
 */
int check_maze_size(FILE *file, int rows, int cols)
{
    int row_counter=0;
    int col_counter=0;
    int c;
    /*Skips row with size info*/
    while ((c = fgetc(file)) != '\n');

    while ((c = fgetc(file)) != EOF) {
      col_counter++;
      /* At beginning of new row controls number of cols, null them and increments row_counter*/
      if (c == '\n') {
          row_counter++;
          if (col_counter < cols * 2)
            return 0;
          col_counter = 0;
      }
      /* Controls rows and colomns overflow*/
      if (col_counter >= cols * 2|| row_counter > rows)
        return 0;
    }
    /* if rows number is higher than expected*/
    if (row_counter < rows - 1)
     return 0;
    return 1;
}

/**
 * @brief Map initialization
 * 
 * @param map structure containing map information
 * @param filename file containing map info
 * @return int 1 - if map was succesfully created 0 - if map wasnt created succesfully
 */
int map_ctor(Map *map, FILE *filename)
{
    int c;
    int n=0;
    int rows;
    int cols;

    fscanf(filename, "%d %d", &rows, &cols);

    map->rows = rows;
    map->cols = cols;
    map->cells = malloc(rows*cols);
    if (map->cells == NULL)
      return 0;

    while ((c = fgetc(filename)) != EOF) {
        if (isdigit(c)) {
          map->cells[n] = c;
          n++; 
        }
    }
    return 1;
}

/**
 * @brief Determines if triangle side is border
 * 
 * @param map Structure containing map info
 * @param r row of cell
 * @param c col of cell
 * @param border Side of triangle which is being tested
 * @return true if side is border
 * @return false if side isn't border
 */
bool is_border(Map *map, int r, int c, int border)
{
  if(((map->cells[(r*map->cols) + (c % map->cols)]) & (border)) == 0)
    return false;
  else
    return true;
}

/**
 * @brief Checks if two triangles next to each other have same sides (border or non border)
 * 
 * @param map Structure containing info about map
 * @return true if triangles have same side
 * @return false if triangles dont have same sides
 */
bool check_borders(Map *map)
{
    int row = 0;
    int col = 0;
    for(int i=0; i < (map->rows*map->cols); i++)
    {
        row = i / map->cols;
        col = i % map->cols;
        if (i!=0 && col != 0) {
            if (is_border(map, row, col, LEFT_BORDER) != is_border(map, row, col-1, RIGHT_BORDER)) {
                return false;
            }
        }
        
        if (row != map->rows) {
          if (col % 2 == 1 && row % 2 == 0) {
            if (is_border(map, row, col, FLAT_BORDER) != is_border(map, row+1, col, FLAT_BORDER)) {
                return false;
            }
            else if (col % 2 == 0 && row % 2 == 1) {
              if (is_border(map,row,col,FLAT_BORDER) != is_border(map,row+1,col,FLAT_BORDER)) {
                return false;
              }    
            }
          }
        }
        
    }
    return true;
}

/**
 * @brief Returns start border
 * 
 * @param map Structure containing map info
 * @param row Starting row
 * @param col Starting col
 * @return int Value of starting border or 0 (when you cannot acces there)
 */
int start_border(Map *map, int row, int col)
{
    if (col == 1) {
      if (is_border(map, row - 1, col - 1, LEFT_BORDER)) {
        if (row == 1 || row == map->rows){
          if (is_border(map, row, col, FLAT_BORDER))
            return FLAT_BORDER;
          else
            return 0;
        }
        else
          return 0;
      }
      else
        return LEFT_BORDER;
    }
    else if (col == map->cols) {
      if (is_border(map, row - 1, col - 1, RIGHT_BORDER)) {
        if ((row == 1 || row == map->rows ) && col % 2 == 1) {
          if (is_border(map, row, col, FLAT_BORDER))
            return FLAT_BORDER;
          else
            return 0;
        }
        else 
          return 0;
      }
      else
        return RIGHT_BORDER;
    }
    else if ((row == 1 && (row + col) % 2 == 0) || (row == map ->rows && (row + col) % 2 == 0))
      return FLAT_BORDER;
    return 0;
}

/**
 * @brief Finds and pritns path from starting point to end
 * 
 * @param map Strucutre containing map info
 * @param r starting row
 * @param c starting col
 */
void find_right_path(Map *map,int r, int c)
{
  /* Starting border */
    int border = start_border(map, r, c);

  /* Decrementing r and c bcs of start border function*/
    r = r - 1;
    c = c -1;

    printf("%d,%d\n",r+1,c+1);

  /* NEXT BORDER CHOICE */
    while(1) {
      if ((r+c) % 2 == 1) {
        if(border == FLAT_BORDER) {
          if(is_border(map, r, c, RIGHT_BORDER))
            border = RIGHT_BORDER;
          else {
            if (c+1 <= map->cols -1) {
             c++;
             border = LEFT_BORDER;
             printf("%d,%d\n",r + 1, c + 1);
            }
            else
              break;
          }
        }
        else if (border == RIGHT_BORDER) {
          if (is_border(map, r, c, LEFT_BORDER))
            border = LEFT_BORDER;
          else {
            if (c-1 >= 0) {
              c--;
              border = RIGHT_BORDER;
              printf("%d,%d\n",r + 1, c + 1);
            }
            else
              break;
          }
        }
        else if (border == LEFT_BORDER) {
          if (is_border(map, r, c, FLAT_BORDER))
            border = FLAT_BORDER;
          else {
            if (r + 1 <= map->rows - 1) {
              r++;
              border = FLAT_BORDER;
              printf("%d,%d\n",r + 1, c + 1);
            }
            else
              break;
          }
        }
      }
      // TWISTED TRIANGLE
      if ((r + c) % 2 == 0) {
        if (border == FLAT_BORDER) {
          if (is_border(map, r, c, LEFT_BORDER))
            border = LEFT_BORDER;
          else {
            if (c-1 >= 0) {
              c--;
              border = RIGHT_BORDER;
              printf("%d,%d\n",r + 1, c + 1);
            }
            else
             break;
          }
        }
        else if(border == LEFT_BORDER){
          if(is_border(map, r, c, RIGHT_BORDER))
            border = RIGHT_BORDER;
          else {
           if(c+1 <= map->cols -1) {
              c++;
              border = LEFT_BORDER;
              printf("%d,%d\n",r + 1, c + 1);
            }
            else
               break;
          }
        }
        else if (border == RIGHT_BORDER) {
          if (is_border(map, r, c, FLAT_BORDER))
            border = FLAT_BORDER;
          else {
            if (r - 1 >= 0) {
              r--;
              border = FLAT_BORDER;
              printf("%d,%d\n",r + 1, c + 1);
            }
            else
              break;
          }
        }
      }
   }
}

/**
 * @brief Finds and prints path with left hand rule
 * 
 * @param map Structure containing map info
 * @param r starting row
 * @param c starting col
 */
void find_left_path(Map *map, int r, int c)
{
  /* Starting border */
  int border = start_border(map, r, c);

  /* Decrementing r and c bcs of start border function*/
  r = r - 1;
  c = c -1;

  printf("%d,%d\n",r+1,c+1);

  while(1){
    if((r + c) % 2 == 1) {
      if(border == RIGHT_BORDER) {
        if(is_border(map, r, c, FLAT_BORDER))
         border = FLAT_BORDER;
        else {
         if (r + 1 <= map->rows - 1) {
          r++;
          border = FLAT_BORDER;
          printf("%d,%d\n",r + 1, c + 1);
         }
         else
          break;
        }
      }
      else if (border == FLAT_BORDER) {
        if (is_border(map, r, c, LEFT_BORDER))
          border = LEFT_BORDER;
        else {
          if (c - 1 >= 0) {
            c--; 
            border = RIGHT_BORDER;
            printf("%d,%d\n", r + 1, c + 1);
          }
          else
            break;
        }
      }
      else if (border == LEFT_BORDER) {
        if (is_border(map,r,c,RIGHT_BORDER))
          border = RIGHT_BORDER;
        else {
          if (c + 1 <= map->cols -1) {
            c++;
            border = LEFT_BORDER;
            printf("%d,%d\n", r + 1, c + 1);
          }
          else
            break;
        }
      }
    }
       // TWISTED TRIANGLE
    if ((r + c) % 2 == 0) {
      if (border == LEFT_BORDER) {
        if (is_border(map,r,c,FLAT_BORDER))
          border = FLAT_BORDER;
        else {
          if (r - 1 >= 0) {
            r--;
            border = FLAT_BORDER;
            printf("%d,%d\n", r + 1, c + 1);
          }
          else
            break;
        }
      }
      else if (border == RIGHT_BORDER) {
        if (is_border(map, r, c, LEFT_BORDER))
          border = LEFT_BORDER;
        else {
          if (c - 1 >= 0) {
            c--;
            border = RIGHT_BORDER;
            printf("%d,%d\n", r + 1, c + 1);
          }
          else
            break;
        }
      }
      else if (border == FLAT_BORDER) {
        if (is_border(map, r , c, RIGHT_BORDER))
          border = RIGHT_BORDER;
        else {
          if (c + 1 <= map->cols - 1) {
            c++;
            border = LEFT_BORDER;
            printf("%d,%d\n", r + 1, c + 1);
          }
          else 
            break;
        }
       }
     }
  }
}

/**
 * @brief Checks arguments
 * 
 * @param argc Number of arguments
 * @param argv Argument
 * @param map Structure containing info about map
 * @return true if arguments passed the test
 * @return false if arguments didnt pass the test
 */
bool check_args(int argc, char *argv[],Map *map)
{
  if (argc <= 5) {
    if (argc == 2) {
      if (!strcmp(argv[1], "--help")) {
        print_help();
        return true;
      }
      else
        ERR("Bad argument input");
    }
    else if (argc == 3) {
      if (!strcmp(argv[1], "--test")) {
        if (check_map_validity(argv[2])) {
          FILE *fr = fopen(argv[2],"r");
          if (!fr) {
            printf("Invalid\n");
            return false;
          }
          if (map_ctor(map,fr)) {
            if (check_borders(map)) {
              printf("Valid\n");
              fclose(fr);
              return true;
            }
            else {
            printf("Invalid\n");
            free(map->cells);
            fclose(fr);
            return false;
            }
          }
          else {
            printf("Invalid\n");
            fclose(fr);
            return false;
          }
        }
        else {
          printf("Invalid\n");
          return false;
        }
      }
      else {
        ERR("Bad arguments");
        return false;
      }
    }
    else if (argc == 5) {
      FILE *fr = fopen(argv[4],"r");
      if (!fr) {
        ERR("File cannot be opened\n");
        return false;
      }
      if (!strcmp(argv[1], "--lpath") || !strcmp(argv[1], "--rpath")) {
        if (is_num(argv[2]) && is_num(argv[3])) {
          fclose(fr);
          return true;
        }
        else {
          fclose(fr);
          ERR("Bad arguments\n");
        }
      }
      else {
        fclose(fr);
        ERR("Bad arguments\n");
      }
    }
  }
  return false;
}

/**
 * @brief Checks if string contains only numbers
 * 
 * @param str checked string
 * @return true if string contains only numbers
 * @return false if string contains something else then only numbers
 */
bool is_num(char *str){
  for (int i = 0;str[i] != '\0'; i++) {
    if (!isdigit(str[i]))
      return false;
  }
  return true;
}

/**
 * @brief Prints help
 * 
 */
void print_help()
{
  printf("--help\n\n");
  printf("Spusti napovedu\n\n\n");
  printf("--test nazev_souboru.txt\n\n");
  printf("Otestuje zda soubor obsahuje validni mapu.\nNazev souboru voli uzivatel\n\n\n");
  printf("--lpath R C nazev_souboru.txt\n\n");
  printf("Najde cestu bludistem pomoci pravidla leve ruky\n");
  printf("R je hodnota pocatecniho radku a C je hodnota pocatecniho sloupce\n");
  printf("Nazev souboru voli uzivatel\n\n\n");
  printf("--rpath R C nazev_souboru.txt\n\n");
  printf("Najde cestu bludistem pomoci pravidla leve ruky\n");
  printf("R je hodnota pocatecniho radku a C je hodnota pocatecniho sloupce\n");
  printf("Nazev souboru voli uzivatel\n\n\n");
} 




