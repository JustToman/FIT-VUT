#include "proj2.h"

 

int main(int argc, char *argv[])
{
   semaphore_unlink(); // Unlinks semaphores at start of program (if somethink unexpected happened and semaphores remained unlinked )

   args_t args;

   /* Checking for errors */
   if(get_args(argc,argv,&args) == PARSE_ERROR)
   {
     fprintf(stderr,"Parse error\n");  
     exit(PARSE_ERROR);
   }

   /*  Initializing shared memory and checking for errors */
   switch(init(shared_mem))
   {
       case MMAP_ERROR:
            fprintf(stderr,"Shared memoryh allocation failed\n");
            exit(MMAP_ERROR);
            break;
       case SEM_ERROR:
             munmap(shared_mem,sizeof(*(shared_mem)));
             fprintf(stderr,"Semaphore  opening failed\n");
             exit(SEM_ERROR);
             break;
       case PARSE_ERROR:
             munmap(shared_mem,sizeof(*(shared_mem)));
             semaphore_close();
             semaphore_unlink();
             fprintf(stderr,"File  opening failed\n");
             exit(PARSE_ERROR);
             break;
       default:
             break;
   }

   pid_t I[args.PI]; // Creating array of immigrant pids
   srand(time(NULL)); // rand seed
   
    /* Child process - generator */
   pid_t imm_generator = fork();

   if( imm_generator == 0 )
       generate_immigrants(I,args);
   else if( imm_generator > 0)  //Parent
   {
       pid_t judge_id = fork();
       
       if(judge_id == 0)
        judge(args);
       else if( judge_id < 0)
       {
            fprintf(stderr,"Fork error\n");
            clean_up();
            shared_mem->fork_error=true;
            exit(FORK_ERROR); 
       }
          
   }
   
   /* Waiting for all processes to end*/
    for(int i = 0; i < args.PI + 2; i++)
    {
        wait(NULL);
    }


    clean_up(); // Cleaning up shared_mem, closing and unlinking sems
    return NO_ERROR;
}

/**
 * @brief Get the args object
 * 
 * @param argc number of arguments
 * @param argv array of arguments
 * @param args Structure where arguments are saved
 * @return NO_ERROR - if parsing is successful PARSE_ERROR - if error ocurred
 */
int get_args(int argc, char*argv[],args_t *args)
{
    /* Saving args adresses to array for looping */
    int *args_arr[5] = {NULL};
    args_arr[0] = &args->PI;
    args_arr[1] = &args->IG;
    args_arr[2] = &args->JG;
    args_arr[3] = &args->IT;
    args_arr[4] = &args->JT;

    if(argc == 6)
    {
      for(int i = 0; i < 5; i++)
      {
          char *fail_check_char = NULL;               //variable used to fail_check
          *args_arr[i] = strtol(argv[i+1],&fail_check_char,10); // Converting 
          if(check_arg_validity(*fail_check_char) != NO_ERROR) // Checking if argument is valid
            return PARSE_ERROR;

        /* Checking args max and min values*/
          if(i == 0 && (*args_arr[i] <= 0))  // if PI <= 0
            return PARSE_ERROR;

          if( i > 0 && (*args_arr[i] < MIN_DELAY_MS || *args_arr[i] > MAX_DELAY_MS)) 
            return PARSE_ERROR;
      }
      return NO_ERROR;
    }
    return PARSE_ERROR;
}

/**
 * @brief Check if args are valid
 * @param last_char_ptr Last character of string containing number.. should be '\0'
 * @return 1 - error 0 - ok
 */
int check_arg_validity(char last_char_ptr)
{
   return (last_char_ptr == '\0') ? NO_ERROR:PARSE_ERROR; 
}

/**
 * @brief Creates shared memory
 * 
 * @return MMAP_ERROR if creating shard memory failed,  NO_ERROR - if creating shared memory was successful
 */
int Shared_mem_create()
{
    return ((shared_mem = mmap(NULL,sizeof(*(shared_mem)),PROT_WRITE | PROT_READ,MAP_SHARED | MAP_ANONYMOUS,-1,0)) == MAP_FAILED) ? MMAP_ERROR:NO_ERROR;
}

/**
 * @brief Opens semaphores
 * 
 * @return SEM_ERROR if semaphore opening failed, NO_ERROR - if semaphores opening was successful
 */
int semaphores_open()
{
    if((shared_mem->s_access = sem_open(S_ACCESS,O_CREAT | O_EXCL, 0666,1)) == SEM_FAILED)
        return SEM_ERROR;
    if((shared_mem->s_all_signed_in = sem_open(S_ALL_SIGNED_IN,O_CREAT | O_EXCL, 0666,1)) == SEM_FAILED)
        return SEM_ERROR;
    if((shared_mem->s_confirmed= sem_open(S_CONFIRMED,O_CREAT | O_EXCL, 0666,1)) == SEM_FAILED)
        return SEM_ERROR;
    if((shared_mem->s_mutex= sem_open(S_MUTEX,O_CREAT | O_EXCL, 0666,1)) == SEM_FAILED)
        return SEM_ERROR;
    if((shared_mem->s_write = sem_open(S_WRITE,O_CREAT | O_EXCL, 0666,1)) == SEM_FAILED)
        return SEM_ERROR;
    
    return NO_ERROR;
}

/**
 * @brief Closes and unlinks semaphores, closes file, deallocating shared memory
 * 
 * Calls semaphore_unlink(),semaphore_close()
 */
void clean_up()
{
    semaphore_unlink();
    semaphore_close();
    munmap(shared_mem,sizeof(*(shared_mem)));
    fclose(fw);
}

/**
 * @brief Closes semaphores
 * 
 */
void semaphore_close()
{
    sem_close(shared_mem->s_access);
    sem_close(shared_mem->s_mutex);
    sem_close(shared_mem->s_confirmed);
    sem_close(shared_mem->s_all_signed_in);
    sem_close(shared_mem->s_write);    
}

/**
 * @brief Unlinks semaphores
 * 
 */
void semaphore_unlink()
{
    sem_unlink(S_WRITE);
    sem_unlink(S_ACCESS);
    sem_unlink(S_CONFIRMED);
    sem_unlink(S_ALL_SIGNED_IN);
    sem_unlink(S_MUTEX); 
}

/**
 * @brief Initializer
 * 
 * Opens semaphores,allocates shared_mem, initalizes shared variables
 * 
 * @return int MMAP_ERROR - shared_mem allocation error, SEM_ERROR - semaphore allocation error, PARSE_ERROR - file opening error, NO_ERROR - everything ok
 */
int init()
{
    if(Shared_mem_create() == MMAP_ERROR)
        return MMAP_ERROR;
    if(semaphores_open() == SEM_ERROR)
        return SEM_ERROR;
    if(!(fw = fopen("proj2.out","w")))
        return PARSE_ERROR;
    shared_mem->judge_in = 0;
    shared_mem->counter = 0;
    shared_mem->NB = 0;
    shared_mem->NC = 0;
    shared_mem->NE = 0;
    shared_mem->judge_on_hold=0;
    shared_mem->fork_error=0;
    sem_wait(shared_mem->s_confirmed);
    sem_wait(shared_mem->s_all_signed_in);
    return NO_ERROR;
}

/**
 * @brief Generates imigrant processes
 * 
 * @param P array of immigrants
 * @param args structure containing arguments
 */
void generate_immigrants(pid_t P[], args_t args)
{
    for(int i = 0; i < args.PI; i++)
    {
        P[i] = fork();  // fork for immingrant

        if(P[i] == 0)
        {
            immigrant(i,args);
        }
        else if(P[i] > 0)
        {
             srand(time(NULL)); // rand seed
             if(shared_mem->fork_error)
              exit(FORK_ERROR);

            if (args.IG != 0)
              usleep(GET_RAND_NUM(args.IG) * 1000);  // sleep
        }
        else
        {
            fprintf(stderr,"Fork error\n");
            clean_up();
            shared_mem->fork_error=true;
            exit(FORK_ERROR);  
        }
        
    }
}

void immigrant(int imm_n,args_t args)
{

     srand(time(NULL)); // rand seed

    if(shared_mem->fork_error)
     exit(FORK_ERROR);


 /* Starting immigrant instance */ 
   sem_wait(shared_mem->s_write);
   
   shared_mem->counter++;
   fprintf(fw,"%d:\t IMM %d: \t starts\n",shared_mem->counter,imm_n + 1);
   fflush(NULL);
   
   sem_post(shared_mem->s_write);
  

   /* Entering hall*/
   sem_wait(shared_mem->s_access);
   sem_wait(shared_mem->s_write);
   shared_mem->NE++;
   shared_mem->NB++;
      
   shared_mem->counter++;
   fprintf(fw,"%d:\t IMM %d: \t enters \t: %d \t : %d \t :% d \n",shared_mem->counter,imm_n + 1,shared_mem->NE,shared_mem->NC,shared_mem->NB);
   fflush(NULL);
     
   sem_post(shared_mem->s_write);
   sem_post(shared_mem->s_access);
     


   /* Immigrant registration */

   sem_wait(shared_mem->s_mutex);
   sem_wait(shared_mem->s_write);

   shared_mem->NC++;
   shared_mem->counter++;
   fprintf(fw,"%d:\t IMM %d: \t checks \t: %d \t : %d \t :% d \n",shared_mem->counter,imm_n + 1,shared_mem->NE,shared_mem->NC,shared_mem->NB);
   fflush(NULL);

    /* If judge is waiting for last immigrant*/
    int all_signed_val;
    sem_getvalue(shared_mem->s_all_signed_in,&all_signed_val);
    if(shared_mem->NC == shared_mem->NE && shared_mem->judge_on_hold == 1 && !all_signed_val)
        sem_post(shared_mem->s_all_signed_in);

   sem_post(shared_mem->s_write);
   sem_post(shared_mem->s_mutex);


   /* If immigrant didn't check before judge entered building*/
   sem_getvalue(shared_mem->s_all_signed_in,&all_signed_val);
   if(shared_mem->judge_in && shared_mem->NC != shared_mem->NE && all_signed_val)
    sem_wait(shared_mem->s_all_signed_in);
   

   sem_wait(shared_mem->s_confirmed);
   shared_mem->confirmed++;
   sem_post(shared_mem->s_confirmed);

   
   /* wants certificate*/
   sem_wait(shared_mem->s_write);
   
   shared_mem->counter++;
   fprintf(fw,"%d:\t IMM %d: \t wants certificate \t: %d \t : %d \t :% d \n",shared_mem->counter,imm_n + 1,shared_mem->NE,shared_mem->NC,shared_mem->NB);
   fflush(NULL);
   
   sem_post(shared_mem->s_write);

    usleep(GET_RAND_NUM(args.IT) * 1000);  // Sleeps before getting certificated

   /* Gets certificate */
   sem_wait(shared_mem->s_write);

   shared_mem->counter++;
   fprintf(fw,"%d:\t IMM %d: \t got certificate \t: %d \t : %d \t :% d \n",shared_mem->counter,imm_n + 1,shared_mem->NE,shared_mem->NC,shared_mem->NB);
   fflush(NULL);

   sem_post(shared_mem->s_write);

   /* Waiting for judge to leave*/
   sem_wait(shared_mem->s_access);
   sem_wait(shared_mem->s_write);

   shared_mem->NB--;

   shared_mem->counter++;
   fprintf(fw,"%d:\t IMM %d: \t leaves \t: %d \t : %d \t :% d \n",shared_mem->counter,imm_n + 1,shared_mem->NE,shared_mem->NC,shared_mem->NB);
   fflush(NULL);

   sem_post(shared_mem->s_write);
   sem_post(shared_mem->s_access);

   exit(NO_ERROR);
}

void judge(args_t args)
{
  int done_cases = 0; // Counter to check if judge confirmed all immigrants
  while( done_cases != args.PI)
  {
   srand(time(NULL)); // rand seed

   usleep(GET_RAND_NUM(args.JG) * 1000);  // Sleeps before entering
  
   /* Wants to enter */
   sem_wait(shared_mem->s_write);

   shared_mem->counter++;
   fprintf(fw,"%d:\t JUDGE:  \t wants to enter \t \n",shared_mem->counter);
   fflush(NULL);

   sem_post(shared_mem->s_write);

   /* Enters  - judge locks access and registration before he leaves*/
 
   sem_wait(shared_mem->s_access);
   sem_wait(shared_mem->s_mutex);

   shared_mem->judge_in = true;  // Signals that judge entered building

   sem_wait(shared_mem->s_write);

   shared_mem->counter++;
   fprintf(fw,"%d:\t JUDGE: \t enters \t: %d \t : %d \t :% d \n",shared_mem->counter,shared_mem->NE,shared_mem->NC,shared_mem->NB);
   fflush(NULL);

   sem_post(shared_mem->s_write);


   /* Waiting for immigrants to check*/

   if(shared_mem->NE > shared_mem->NC)
   {

     sem_wait(shared_mem->s_write);
     
     shared_mem->counter++;
     fprintf(fw,"%d:\t JUDGE: \t waits for imm \t: %d \t : %d \t :% d \n",shared_mem->counter,shared_mem->NE,shared_mem->NC,shared_mem->NB);
     fflush(NULL);

     sem_post(shared_mem->s_write);

     shared_mem->judge_on_hold=1;  //Signals that judge is waiting for immigrants

     sem_post(shared_mem->s_mutex); // Letting immigrants register
     sem_wait(shared_mem->s_all_signed_in); //Waiting for immigrants to sign
     sem_wait(shared_mem->s_mutex); // Locking registration
     shared_mem->judge_on_hold=0; // On hold == 0 -> Judge isn't waiting anymore
   }

     /* Confirmation started */
     sem_wait(shared_mem->s_write);
     
     shared_mem->counter++;
     fprintf(fw,"%d:\t JUDGE: \t starts confirmation \t: %d \t : %d \t :% d \n",shared_mem->counter,shared_mem->NE,shared_mem->NC,shared_mem->NB);
     fflush(NULL);
 
     usleep(GET_RAND_NUM(args.JT) * 1000); // Sleeps before ending confirmation
     
     done_cases += shared_mem->NC; // incresasing value of done cases by value of confirmed immigrants
     shared_mem->NC = 0; // Resets counter of checked immigrants
     shared_mem->NE = 0; // Resets counter of entered immigrants
     
     shared_mem->counter++;
     fprintf(fw,"%d:\t JUDGE: \t ends confirmation \t: %d \t : %d \t :% d \n",shared_mem->counter,shared_mem->NE,shared_mem->NC,shared_mem->NB);
     fflush(NULL);
    
     int conf_val;
     sem_getvalue(shared_mem->s_confirmed,&conf_val);

     sem_post(shared_mem->s_write); 
   
     sem_post(shared_mem->s_confirmed);
     /* Confirmation ended */

     usleep(GET_RAND_NUM(args.JT) * 1000); // Sleeps before he leaves

    
     sem_wait(shared_mem->s_confirmed);

     sem_wait(shared_mem->s_write);
     
     shared_mem->counter++;
     fprintf(fw,"%d:\t JUDGE: \t leaves \t: %d \t : %d \t :% d \n",shared_mem->counter,shared_mem->NE,shared_mem->NC,shared_mem->NB);
     fflush(NULL);
    
     sem_post(shared_mem->s_write);
    
     shared_mem->judge_in = false; // Judge isn't in building anymore
     

     sem_post(shared_mem->s_mutex); // Unlocks registration
     sem_post(shared_mem->s_access); // Unlocks access to building
     
  }
    sem_post(shared_mem->s_confirmed);
     /* If judge confirmed all immigrants, he finishes*/
     sem_wait(shared_mem->s_write);
     
     shared_mem->counter++;
     fprintf(fw,"%d:\t JUDGE: \t finishes \n",shared_mem->counter);
     fflush(NULL);
     
     sem_post(shared_mem->s_write);
     exit(NO_ERROR);  
}








