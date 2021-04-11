# IOS 2nd project 

## Goal
Solution of adjusted Faneuil Hall synchronization problem ([https://greenteapress.com/semaphores/LittleBookOfSemaphores.pdf]), only with processes.

## Specifications
### Language
C99
### OS
Linux

## Execution
- make
- ./proj2 PI IG JG IT JT

PI - Number of Immigrants (Must be > 0)

IG - Immigrant delay on spawn \[ms\] (Must be between 0 and 2000)

JG - Judge sleep before entering hall \[ms\] (Must be between 0 and 2000)

IT - Immigrant delay before getting certificated \[ms\] (must be between 0 and 2000)

JT - Judge delay before ending certificate confirmation and leaving hall \[ms\] (must be between 0 and 2000)
