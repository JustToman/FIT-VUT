// tail.h
// Řešení IJC-DU2, příklad  b) 15.4.2020
// Autor: Roman Popelka, FIT
// Přeloženo: gcc 7.5
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define MAX_LINE_LENGTH 1023  
#define DEFAULT_LINES_COUNT 10 

/**
 * @brief Structure containing info about file and given arguments
 * 
 */
typedef struct tail
{
	long lines_to_be_printed_;  // Number of lines to be printed 
    long n_of_lines;            // Number of lines in file
    FILE *file_;                // read file
    char n_param_prefix_;       // prefix  '+' or 0
    char *output_string_;  // 2D array stored as 1D
}Tail;


/**
 * @brief Loads parameters to  structure Tail
 * 
 * argc == 0 || argc == 1  -  Ignored
 * argc ==  2  - paramter argv[1] == filename.  file opens in load parameters too 
 * argc == 3  - parameter -n was given.. Checking number prefixes + and - and loading number of lines in tail.lines_to_be_printed_ Runs on stdin
 * argc == 4 - same as 3  + parameter argv[3] == filename
 * 
 * @param argc number of arguments
 * @param argv arguments stored as strings
 * @param tail variable of structure Tail
 * @return true if parameters parsign was succesful
 * @return false  if something was unsucssessful
 */
bool load_parameters(const int argc,char *argv[], Tail * tail);

/**
 * @brief Opens file for read and returns it
 * 
 * @param filename name of file to open
 * @return FILE* file pointer
 */
FILE *open_file_for_read(const char *filename);

/**
 * @brief Checks if file was opened successfully
 * 
 * @param file pointer to tested file
 * @return true file was opened succesfuly => file != NULL
 * @return false file wasn't opened succesfully
 */
bool check_file_load(FILE *file);

/**
 * @brief Exits with error printing formated string to stderr
 * 
 * @param format format of message
 * @param ... message arguments
 */
void exit_error(const char * format,...);

/**
 * @brief Prints lines store in tail->output_string_
 * 
 * @param tail pointer to structure Tail

void print_lines_f(Tail * tail);
*/

/**
 * @brief Allocates memory at tail->output_string_ 
 * 
 * Allocated memory depends on number prefix (+)
 * 
 * @param tail Pointer to structure Tail
 * @return true Allocation was successful
 * @return false Allocation was unsuccesful
 */
bool alloc_str_arr(Tail *tail);

/**
 * @brief Prints lines stored in tail->output_string_ 
 * 
 * Depends on number prefix (+)
 * 
 * @param tail Pointer to structure Tail
 */
void print_lines(Tail *tail);

/**
 * @brief Stores last -n lines to tail->output_string_
 * 
 * @param tail pointer to structure Tail
 */
void store_lines(Tail *tail);

/**
 * @brief Checks if line length exceeded implementation limit equal to MAX_LINE_LENGTH
 * 
 * @param last_char number of last character before '\0'
 * @return true line length limit is exceeded
 * @return false line length limit isn't exceeded
 */
bool is_limit_exceeded(char last_char);


/**
 * @brief If line length is exceeded first time prints err msg to stderr
 * 
 * @param str addres of string which contains line
 * @param line_length_exceeded 1 = was exceeded already 0 = wasn't exceeded
 */
void print_line_overflow(char *str,bool line_length_exceeded);

/**
 * @brief If line length limit is exceeded, skips to EOL
 * 
 * @param is_limit_exceeded bool value containing info if line exceeded limit or not true - exceeded
 * @param file read file
 */
void jmp_to_eol(bool is_limit_exceeded ,FILE *file);


