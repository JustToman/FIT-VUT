#Makefile
#Řešení IJC-DU2, příklad a,b) 15.4.2020
#Autor: Roman Popelka, FIT

CC=gcc
CFLAGS= -std=c99 -Wall -Wextra -pedantic -g -O2
.PHONY: clean all pack

LIB_OBJS= htab_begin.o htab_bucket_count.o htab_clear.o htab_end.o htab_erase.o htab_find.o htab_free.o htab_hash_fun.o htab_init.o htab_iterator_get_key.o htab_iterator_get_value.o htab_iterator_next.o htab_iterator_set_value.o htab_lookup_add.o htab_size.o

LIB_SRC= htab_begin.c htab_bucket_count.c htab_clear.c htab_end.c htab_erase.c htab_find.c htab_free.c htab_hash_fun.c htab_init.c htab_iterator_get_key.c htab_iterator_get_value.c htab_iterator_next.c htab_iterator_set_value.c htab_lookup_add.c htab_size.c

all:wordcount wordcount-dynamic tail


tail: tail.h tail.c
	$(CC) $(CFLAGS) -o $@ tail.c

wordcount-dynamic: libhtab.so io.o wordcount.c
	$(CC) $(CFLAGS) -fPIC -o $@ io.o wordcount.c -L. -lhtab

wordcount: libhtab.a io.o wordcount.c
	$(CC) $(CFLAGS) -o $@ -static io.o wordcount.c -L. -lhtab

libhtab.so: $(LIB_SRC)  htab_t.h htab_item.h htab.h
	$(CC) $(CFLAGS) -c -fPIC $(LIB_SRC)
	$(CC) $(CFLAGS) -fPIC -shared $(LIB_OBJS) -o $@

libhtab.a: $(LIB_OBJS) htab_t.h htab_item.h htab.h
	ar rcs $@ $(LIB_OBJS)
	ranlib $@

pack: 
	zip xpopel24.zip *.c *.h Makefile

clean:
	rm *.o *.a *.so wordcount wordcount-dynamic tail xpopel24.zip


