/**
 * @file proj1.c
 * @author Roman Popelka(xpopel24)
 * @brief Searching contacts containing given set of numbers
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_LENGTH 101      //Maximum array size
#define MAX_CONTACTS 70     // Maximum contacts

// Structure for contacts
typedef struct {
	char name[MAX_LENGTH];
	char number[MAX_LENGTH];

} CONTACT;

/* Function prototypes  */
int is_num(char *str);
int cmp_contact(char *arg_str, char *str);
char* conv_str(char *str);
char* str_to_lower(char *str);
void err_msg(char *str);
void print_contacts(CONTACT contact[], int n);
int found_contact(char *num, CONTACT contact[], int n);

int main(int argc, char *argv[])
{
	CONTACT contact[MAX_CONTACTS];
	char temp_buffer[MAX_LENGTH]; // Temp. buffer which stores actual row of input
	int n = 0;                    // Iteration counter
	int arr_indx = 0;             // Current array index

    /*Printing contacts without given argument (all contacts on input) */
	if (argc<2) {
	  argv[0] = NULL;
	  while (fgets(temp_buffer, MAX_LENGTH + 1, stdin)) { 
	    if (strlen(temp_buffer) >= (MAX_LENGTH) || strlen(temp_buffer) <= 0)
		{
		  err_msg("String size of limits");
		  return 1;
		}
		if(n >= MAX_CONTACTS * 2) {       // Determines if there are more contacts than the border at the input. (MAX_CONTACTS multiplied by 2 because 2 iterations == 1 contact fill)            
			err_msg("Too many contacts given");
			return 1;
		}
		
		temp_buffer[strcspn(temp_buffer, "\n")] = '\0';	 // Changing Newline character in string to NULL character, because of  data listing format (using strlen may be better)
		
		/* Assigns number to struct  */
		if ((n%2) == 1) {                                                       
		  strcpy(contact[arr_indx].number, temp_buffer);
          arr_indx++;
		}                                 

		/* Assigns number to struct */		
		else if ((n%2) == 0)                                                  
		  strcpy(contact[arr_indx].name, temp_buffer);   	

		n++;
	  }
	}

	/* Find and print contacts which match the argument */
	else {		
	  if (!is_num(argv[1])) {        // Checks if argument is number
	    err_msg("Error: Argument doesn't contain only numbers\n");
		return 1;
	  }

	  while (fgets(temp_buffer, MAX_LENGTH + 1, stdin) && !feof(stdin)) { 

		if (strlen(temp_buffer) >= MAX_LENGTH || strlen(temp_buffer) <= 0) {          // Array length check
		  err_msg("String size of limits.\n");
		  return 1;
		}
		if (n > MAX_CONTACTS * 2) {        // Determines if there are more contacts than the border at the input. (MAX_CONTACTS multiplied by 2 because 2 iterations == 1 contact fill)
		  err_msg("Too many contacts given \n");
		  return 1;
		}       
        
		temp_buffer[strcspn(temp_buffer, "\n")] = '\0';	 // Changing Newline character in string to NULL character, because of  data listing format

		if ((n%2) == 1) {                                                        
		  strcpy(contact[arr_indx].number, temp_buffer);
    	  
		  if(found_contact(argv[1], contact, arr_indx))  // if contact matches, index increments array index by one
            arr_indx++;   
		}
		else if ((n%2) == 0)                                                  
		  strcpy(contact[arr_indx].name, temp_buffer);   
		
		n++;	
		}
	}

	/* Contact doesn't have assigned number*/
	if (n%2 == 1) {
	  err_msg("Contact doesn't have assigned number\n");
	  return 1;
	}
	   
	/* No contacts found check*/
	if (arr_indx == 0)
	  printf("Not found\n");

	/* Print contacts which mach the argument */
	print_contacts(contact, arr_indx);
		
	return 0;
}

/**
 *@brief Testing if string contains only numbers
 * 
 * If string contains only numbers, returns 1
 * 
 * @param str   Tested string
 * @return int  1 - test successful , 0 - test unsuccessful
 */
int is_num(char *str)
{
	for (int i = 0; str[i] != '\0'; i++) {
	  if (!isdigit(str[i]))
	    return 0;
	}	
	return 1;
}

/**
 * @brief Returns 1 if arg_str is substring of str number expression
 * 
 * More about letters to number conversion at conv_str function
 * 
 * @param arg_str given string of numbers in main argument
 * @param str     given string of characters to compare with argument
 * @return int      Return: 1 if str is substring of arg_str; 0 if it is not
 */
int cmp_contact(char *arg_str, char *str)
{
	char str_num_ex[strlen(str)];                                       // Variable for saving number expression of name or number

	strcpy(str_num_ex, str);                                            // Copying cause I dont want original string to overwrite
	if (strlen(str) >= strlen(arg_str)) {                              // Checks if Contact length is longer than argument
	  if (strstr(conv_str(str_num_ex), arg_str) == NULL)                  // Determines if arg_str is substring of str number expression
	    return 0;
	  else
	    return 1;
	}
	else
		return 0;
}

/**
 * @brief Returns string of numbers converted as specified below
 * 
 * Returns string of numbers where each number represents one of alphabetic letters
 * Conversion specification: 2 (abc), 3 (def), 4 (ghi),
 * 5 (jkl), 6 (mno), 7 (pqrs), 8 (tuv), 9 (wxyz), 0 (+)    
 * Changes specified characters and leaves other as they are
 * 
 * @param  str      Input string 
 * @return char*   Converted string
 */
char* conv_str(char *str)
{
	
	str_to_lower(str);
	for (int i=0; str[i] != '\0'; i++) {
	
	  if (str[i] >= 'a' && str[i] <= 'c')
	    str[i] = '2';
	  if (str[i] >= 'd' && str[i] <= 'f')
		str[i] = '3';
	  if (str[i] >= 'g' && str[i] <= 'i')
	    str[i] = '4';
	  if (str[i] >= 'j' && str[i] <= 'l')
		str[i] = '5';
	  if (str[i] >= 'm' && str[i] <= 'o')
		str[i] = '6';
	  if (str[i] >= 'p' && str[i] <= 's')
		str[i] = '7';
	  if (str[i] >= 't' && str[i] <= 'v')
		str[i] = '8';
	  if (str[i] >= 'w' && str[i] <= 'z')
		str[i] = '9';
	  if (str[i] == '+')
		str[i] = '0';
	}
	return str;
}

/**
 * @brief Changes uppercase  to lowercase in string
 * 
 * Changes uppercase to lowercase, leaving all other characters as they are
 * 
 * @param str    Input string 
 * @return char* String where lowercase replaced upercase
 */
char* str_to_lower(char *str)
{
	for (int i=0; str[i] != '\0'; i++) {
	  if (str[i] >= 'A' && str[i] <= 'Z')
	    str[i] = str[i] + ('a' - 'A');       
	}
	return str;
}

/**
 * @brief Prints err msg 
 * 
 * @param str err msg
 */
void err_msg(char *str)
{
	fprintf(stderr, "%s", str);
}

/**
 * @brief Prints contacts which match the argument
 * 
 * @param contact Array of structures CONTACT
 * @param n  Number of contacts
 */
void print_contacts(CONTACT contact[], int n) 
{
	for (int i = 0; i < n; i++) {
	  printf("%s, %s \n", contact[i].name, contact[i].number);
	}
}

/**
 * @brief Returns 1 if contact match the argument
 * 
 * @param num  Number given as argument
 * @param contact Array of structures CONTACT
 * @param n  Index of tested contact
 * @return int 1- matches argument 0- doesn't match the argument
 */
int found_contact(char *num, CONTACT contact[], int n)
{
    if (cmp_contact(num,contact[n].number))
      return 1;
    else if (cmp_contact(num,contact[n].name))
    {
	  return 1;
	}
    else
      return 0;
}
