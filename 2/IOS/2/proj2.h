#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <limits.h>
#include <string.h>
#include <pthread.h> 


#define MAX_DELAY_MS 2000
#define MIN_DELAY_MS 0
#define ARG_NUM 6
#define NO_ERROR 0
#define PARSE_ERROR 1
#define MMAP_ERROR 2
#define SEM_ERROR 3
#define FORK_ERROR 4

#define S_WRITE "/xpopel24.ios.projekt2.s_write"
#define S_MUTEX "/xpopel24.ios.projekt2.s_mutex"
#define S_ALL_SIGNED_IN "/xpopel24.ios.projekt2.s_all_signed_in"
#define S_CONFIRMED "/xpopel24.ios.projekt2.s_confirmed"
#define S_ACCESS "/xpopel24.ios.projekt2.s_access"



#define GET_RAND_NUM(num) ((num != 0)? (rand() % (num) + 1):0)

typedef struct shared_mem
{

    sem_t *s_write;
    sem_t *s_mutex;
    sem_t *s_confirmed;
    sem_t *s_access;
    sem_t *s_all_signed_in;
    bool fork_error;
    int judge_on_hold;
    int counter;
    int NE;
    int NC;
    int NB;
    bool judge_in;
    int confirmed;
}Shared_mem;

 
Shared_mem *shared_mem;
FILE *fw = NULL;


/* Structure containing parameter values */
typedef struct args
{
    int PI;  // Number of procs made in immigrant category >=1
    int IG;  // >= MIN_DELAY_MS && <= MAX_DELAY_MS
    int JG;  // >= MIN_DELAY_MS && <= MAX_DELAY_MS
    int IT;  // >= MIN_DELAY_MS && <= MAX_DELAY_MS
    int JT;  // >= MIN_DELAY_MS && <= MAX_DELAY_MS

}args_t;


int get_args(int argc, char*argv[],args_t *args);
int check_arg_validity(char last_char_ptr);
int Shared_mem_create();
int semaphores_open();
void clean_up();
int init();
void generate_immigrants(pid_t P[], args_t args);
void immigrant(int imm_n,args_t args);
void judge(args_t args);
void semaphore_unlink();
void semaphore_close();

