#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

/* Macros definition*/
#define I0 1e-12
#define UT 0.0258563
#define MAX_ITER 200

#define ERR(str) (fprintf(stderr,"%s",(str)))

/* Functions prototypes*/
int check_arguments(char **str);
int end_condition(double yValue, double ypValue,double eps);
double calcEQ(double u0, double r, double up);
double diode(double u0, double r, double eps);



int main(int argc, char *argv[])   // argv[1] - u0 , argv[2] - r, argv[3] - eps
{
  if (argc != 4) {
    ERR("error: invalid arguments\n");
    return 1;
  }
  if (!check_arguments(argv))
  {
    ERR("error: invalid arguments\n");
    return 1;
  }
  
  /* Converting arguments to double data types*/
  double u0 = atof(argv[1]);
  double r = atof(argv[2]);
  double eps = atof(argv[3]);
  
  /**/
  double up = diode(u0, r, eps);       
  double ip = I0 * (exp(up / UT) - 1);

  printf("Up=%g V\nIp=%g A\n", up, ip);

  return 0;
}

/**
 * @brief Checks if arguments are numbers and if they are, chcecks if they have allowed values
 * 
 * @param str argument
 * @return int 1 - arguments meets the conditions 0 - failed
 */
int check_arguments(char **str)
{
  char *eon_ptr = NULL;  //End of number in string pointer
  for (int i = 1; i <= 3; i++) { 
    if (isnan(strtod(str[i], &eon_ptr)) || *eon_ptr != 0) {       //If is NaN or does not containt numbers only 
      return 0;
    }
  }

  if (atof(str[1]) < 0 || atof(str[2]) <= 0 || atof(str[3]) <= 0 || atof(str[3]) > 1)  // If u0<0 || R<=0 || eps >= 0
    return 0;
  return 1;
}

/**
 * @brief Returns 1 if absolute value of a-b is equal or less than epsilon
 * 
 * @param a Previous interval halving value
 * @param b Next interval halving value
 * @param eps epsilon
 * @return 1 or 0 based on condition
 */
int end_condition(double min, double max, double eps)
{
  if (fabs(max - min) <= eps)
    return 1;
  else 
    return 0;	
}


/**
 * @brief Returns result of Shockley equation - IR
 * 
 * @param u0 Ucc
 * @param r Resistance of resistor
 * @param up Voltage on diode
 * @return double 
 */
double calcEQ(double u0, double r, double up)
{
    return (r * I0 * (exp(up / UT) - 1) - u0 + up);
}

/**
 * @brief Returns voltage on diode
 * 
 * @param u0 Ucc
 * @param r Resistance on resistor
 * @param eps epsilon
 * @return double 
 */
double diode(double u0, double r, double eps)
{
  double min = 0.0;
  double max = u0;
  double middle;
  int n = 0;
  while (!end_condition(min, max, eps) && n <= MAX_ITER)    // if epsilon is higher than a-b   or if Iterations are less than MAX_ITER
  {
    middle = (min + max) / 2;              // halfs interval
    if ((calcEQ(middle, r, u0)) == 0) // if middle value is root of equation
    {
      return middle;
    }
    else if (calcEQ(u0, r, middle) * calcEQ(u0, r, min) < 0)  // if middle and a equation result value don't have the same sign
    {
      max = middle;   // b equals middle
    }
    else 
    {
      min = middle;  // else a equals middle
    }

    n++;
  }
  return middle;         //Returns value closest to root of equation (Up)
}
