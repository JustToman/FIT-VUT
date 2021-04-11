// tail.c
// Řešení IJC-DU2, příklad a) 15.3.2020
// Autor: Roman Popelka, FIT
// Přeloženo: gcc 7.5
// Zdrojový soubor tail.c pro vypisování posledních n řádků souboru
// Pokud se argumentem -n nezadá požadovaný počet řádků, nastaví se počet posledních řádků, které se maj implicitně na 10
#include "tail.h"


int main(int argc, char *argv[])
{
    Tail tail = {.lines_to_be_printed_ = 10, .file_ = stdin, .output_string_ = NULL, .n_param_prefix_ = 0};   //Base declaration of tail structure

    /* Loading parameters to structure*/
    if(!load_parameters(argc,argv,&tail))
    {    
        fclose(tail.file_);
        exit_error("tail: Arguments parsing error\n");
    }

    /* Choosing  method to get last lines depending on type .. file or stdin*/
          
        if (!alloc_str_arr(&tail))
            exit_error("tail: Nepodarilo se alokovat misto v pameti\n");
        if (tail.n_param_prefix_ != '+')
            store_lines(&tail);
        
        print_lines(&tail);

    fclose(tail.file_);
    free(tail.output_string_);
    
    return EXIT_SUCCESS;
}

bool load_parameters(const int argc,char * argv[], Tail * tail)
{
    switch (argc)
    {
        case 0: break;
        case 1: break;
        case 2: 
            tail->file_ = open_file_for_read(argv[1]);
            if(!check_file_load(tail->file_))
                return 0;
            break;
        case 4:                                              
            tail->file_ = open_file_for_read(argv[3]);
            if(!check_file_load(tail->file_))
                return 0;
        /* FALLTHROUGH */
        case 3:
            if(strcmp(argv[1],"-n"))
                return 0;
            if(argv[2][0] == '+')                 //Checkin if prefix == +
                tail->n_param_prefix_ = argv[2][0];
            else
                tail->n_param_prefix_ = 0;
            char *num_check = NULL;
            tail->lines_to_be_printed_ = strtol(argv[2],&num_check,10);   // Converting string to number
            if(*num_check != '\0' || tail->lines_to_be_printed_ <= 0)     //Checking if number input is valid.. No letters and >0
                return 0;
            break;
        default:
            return 0;
    }
    return 1;
}

FILE * open_file_for_read(const char *filename)
{
    FILE *fr = fopen(filename,"r");
    return fr;
}

bool check_file_load(FILE *file)
{
    return (file != NULL);
}

void exit_error(const char * format,...)
{
    va_list args;
    va_start(args,format);
    vfprintf(stderr,format,args);
	va_end (args);
	exit(EXIT_FAILURE);
}

bool alloc_str_arr(Tail *tail)
{
    /*  MAX_LINE_LENGTH represents 1 string
        allocating output_string_ is depending on prefix.. If prefix equals '+' just  1*MAX_LINE_LENGTH is allocated because after loading string to variable, it's immidiately printed
        without prefix output_string_ size is allocated to number of lines_to_be_printed * MAX_LINE_LENGTH to store all lines that needs to be printed
    */
    tail->output_string_ = (tail->n_param_prefix_ == '+') ? malloc(1 * MAX_LINE_LENGTH):malloc(tail->lines_to_be_printed_ * MAX_LINE_LENGTH);
    return (!tail->output_string_)?false:true;
}


void store_lines(Tail *tail)
{   
    bool line_length_exceeded = 0;  // true  - if line length limit was already exceeded 
    long n_of_lines = 1; // starts at 1, because later we do % with this value (easier implementing this way) later when saved to tail->n_of_lines the 1 is substracted

    /* Until EOF keeps circulary saving lines */
    for(char *tmp = tail->output_string_; fgets(tmp,MAX_LINE_LENGTH,tail->file_) != NULL; n_of_lines++)
    {
        jmp_to_eol(is_limit_exceeded(*(tmp+1021)),tail->file_); // If line limit is exceeded jumps to \n to continue with next line
        print_line_overflow(tmp,line_length_exceeded); // Prints when hits 1st line that exceeded max line length

    /* Checking if saved lines reached number of lines_to_be_printed and sets buffer to it's starting address to load new lines */
        if(n_of_lines % tail->lines_to_be_printed_  != 0)
            tmp += MAX_LINE_LENGTH;  //  jmp to next element
        else
            tmp -= MAX_LINE_LENGTH * (tail->lines_to_be_printed_ - 1); // jmp to first element
    }

    long last_index = (n_of_lines % tail->lines_to_be_printed_); // contains index, where last line was stored

    /* Fixing order in which are lines stored     Basically it  moves array to right until i reaches value of last_index or until lines_to_be_printed value reaches value of n_of_lines */
    for(long i = 0; (i < last_index - 1) && (tail->lines_to_be_printed_ < n_of_lines - 1) ; i++)
    {
        char tmp_string[MAX_LINE_LENGTH];                    
        strcpy(tmp_string,tail->output_string_);                     // tmp_string = tail->output_string_[0]
        for(long j = 0; j < tail->lines_to_be_printed_ - 1; j ++)    // Loop through whole array tail->output_string_
            strcpy(tail->output_string_ + (j*MAX_LINE_LENGTH),tail->output_string_ + ((j+1)*MAX_LINE_LENGTH));  // tail->output_string[j] = tail->output_string[j+1]
            
        strcpy(tail->output_string_ + ((tail->lines_to_be_printed_-1) * MAX_LINE_LENGTH),tmp_string);  // tail->output_string_[lines_to_be_printed - 1] = tail->output_string_[0]
    }
    tail->n_of_lines = n_of_lines - 1;        // Storing n_of_lines - 1 to tail->n_of_lines
}


void print_lines(Tail *tail)
{
    bool line_length_exceeded = 0; // true - if line length limit  was already exceeded 
    switch(tail->n_param_prefix_)
    {
        case '+':
            for (long i = 0; fgets(tail->output_string_, MAX_LINE_LENGTH, tail->file_) != NULL; i++)
            {
                jmp_to_eol(is_limit_exceeded(*(tail->output_string_+1021)),tail->file_); // If line limit is exceeded jumps to \n to continue with next line
                print_line_overflow(tail->output_string_,line_length_exceeded); // Prints when hits 1st line that exceeded max line length
    
                if(i >= (tail->lines_to_be_printed_ - 1))                           // if i > -n +tail_lines_to_be_printed - 1
                    printf("%s",tail->output_string_);                        // print line
            }
            break;
        default:

            /* If value of tail->lines_to_be_printed is higher than tail->n_of_lines value, lines_to_be_printed is set to tail->n_of_lines*/
            tail->lines_to_be_printed_ = (tail->lines_to_be_printed_ > tail->n_of_lines) ? tail->n_of_lines:tail->lines_to_be_printed_; 

            /* Printing lines */
            for(long i = 0; i < tail->lines_to_be_printed_;i++)
            {
                printf("%s",tail->output_string_ + (i*MAX_LINE_LENGTH)); 
            }
            break;
    } 
}

bool is_limit_exceeded(char last_char)
{
    return (last_char == '\n' || last_char == '\0') ? false:true;
}

void jmp_to_eol(bool is_limit_exceeded ,FILE *file)
{
    int c;
    while(is_limit_exceeded && (c = fgetc(file)) != '\n'); 
}

void print_line_overflow(char *str,bool line_length_exceeded)
{
    if(is_limit_exceeded(*(str+1021)) && !line_length_exceeded)
    {
      line_length_exceeded = true;
      fprintf(stderr,"Line is longer than limit\n");
    }
}

