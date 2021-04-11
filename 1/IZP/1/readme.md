## IZP 1st project

## Description
Searching contacts cotanining given set of numbers

## Specifications
Language: c99

## Execution
gcc -std=c99 -Wextra -Werror -g -pedantic -Wall proj1.c -o proj1

./proj1 <number pattern to be found>    (Contacts data are given on stdin) (If argument isn't passed returns all contacts)
  
### Example
./proj1 7  < Carl 24 CJ 7 
output: CJ, 7
