#Makefile
#Řešení IJC-DU1, příklad a), 12.3.2020
#Autor: Roman Popelka, FIT
#Přeloženo: gcc 7.5

CC=gcc 
CFLAGS= -O3 -std=c11 -pedantic -Wextra -Wall -g 
LDFLAGS = -lm

all: primes primes-i steg-decode

primes: bitset.h primes.c eratosthenes.c error.h                  
	LC_ALL=cs_CZ.utf8 $(CC) $(CFLAGS) -o primes primes.c eratosthenes.c error.c $(LDFLAGS)         
primes-i: bitset.h primes.c eratosthenes.c bitset.c error.h
	LC_ALL=cs_CZ.utf8 $(CC) $(CFLAGS) -DUSE_INLINE -o primes-i primes.c eratosthenes.c bitset.c error.c $(LDFLAGS)        
steg-decode: error.h error.c steg-decode.h steg-decode.c  ppm.h ppm.c eratosthenes.c eratosthenes.h bitset.h
	LC_ALL=cs_CZ.utf8 $(CC) $(CFLAGS) -o steg-decode error.c ppm.c steg-decode.c eratosthenes.c $(LDFLAGS)        

run: primes primes-i
	ulimit -s 100000
	./primes 
	./primes-i

    