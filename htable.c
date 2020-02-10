/********************************************************\           
 * htable.c -- builds a hash table from inputs given by *
 *             htable-main                              *
 *                                                      *   
 * Authors:    James Douglas, Paul Souter,              *
 *             Ryan Swanepoel                           *   
 *                                                      *   
 * Purpose:    to build a hash table using linear       *
 *             probing or double hashing                *
 *                                                      *
\********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "htable.h"
#include "mylib.h"
#include <string.h>

struct htablerec{
    int capacity;
    int num_keys;
    char** keys;
    int *freq;
    int *stats;/*colisions for each key*/
    hashing_t method;
};

#define IS_LINEAR(x) ((NULL == (x)) || (LINEAR_P == (x)->method))
#define IS_DOUBLE(x) ((NULL != (x)) && (DOUBLE_H == (x)->method))

/**
 * Converts a given character pointer into an integer
 *
 * @param word the word to convert.
 * @return result the integer representation of the word.
 */
static unsigned int htable_word_to_int(char *word){
    unsigned int result = 0;
    while (*word != '\0'){
        result = (*word++ + 31*result);
    }
    return result;
}



/**
 * Calculates the step size for the given key
 *
 * @param h the table being used.
 * @param i_key the integer representation of a word.
 *
 * @return the step,1 if linear and claculated if double.
 */
static unsigned int htable_step(htable h, unsigned int i_key){
    if (IS_DOUBLE(h)){
        return 1 + (i_key % (h->capacity - 1));
    }else{
        return 1;
    }
}

/**
 * Builds a new table with a maximum size of capacity,
 * and a hashing style of method
 *
 * @param capacity the maximum size of the hash table.
 * @param method the hashing method to use, linear or double.
 *
 * @return result the new hash table.
 */
htable htable_new(int capacity, hashing_t method) {
    int i;
    htable result = emalloc(sizeof *result);
    if(capacity<1){
        result->capacity = 113;
    }else{
        result->capacity = capacity;
    }
    result->keys = emalloc(result->capacity * sizeof (result->keys));
    result->num_keys = 0;
    result->freq = emalloc(result->capacity * sizeof (result->freq));
    result->stats = emalloc(result->capacity * sizeof(result->stats));
    for(i =0;i<result->capacity;i++){        
        result->freq[i] = 0;
        result->stats[i] = 0;
        result->keys[i] = NULL;
    }
    result->method = method;
    return result;
}

/**
 * frees the memory space of the hash table.
 *
 * @param h the word to table to free.
 */
void htable_free(htable h){
    int i;
    for(i=0;i<h->capacity;i++){
        free(h->keys[i]);
    }
    free(h->keys);

    free(h->freq);
    free(h->stats);
    free(h);
}

/**
 * Attempts to insert a key into the table,
 * if the key already exists increment the frequency.
 * 
 * 
 * @param h the table to add to.
 * @param str the key to add into the table.
 *
 * @return 1 if insert was successful, 0 if not.
 */
int htable_insert(htable h, char *str){
    unsigned int result = htable_word_to_int(str);
    unsigned int hashIndex = result % h->capacity;
    unsigned int step = (htable_step(h,result) % h->capacity);
    
    if (h->freq[hashIndex] == 0){
        h->keys[hashIndex] = erealloc(h->keys[hashIndex], (strlen(str)+1) *
                                      sizeof h->keys[0][0]);
        strcpy(h->keys[hashIndex],str);
        h->freq[hashIndex]++;
        h->num_keys++;
        return 1;
    }else if(strcmp(str,h->keys[hashIndex]) == 0){
        h->freq[hashIndex]++;
        return h->freq[hashIndex];
    }else if(strcmp(str,h->keys[hashIndex]) != 0){
        unsigned int newIndex = hashIndex;
        int numKeys = h->num_keys;
        int i = 0;
        unsigned int colisions =0;
        while(i != h->capacity){
            if(h->freq[newIndex] == 0){
                h->keys[newIndex] = erealloc(h->keys[newIndex],(strlen(str)+1)
                                             * sizeof h->keys[0][0]);
                strcpy(h->keys[newIndex],str);
                h->freq[newIndex]++;
                h->stats[numKeys] = colisions;
                h->num_keys++;
                return 1;
            }else if(strcmp(h->keys[newIndex],str) == 0){
                h->freq[newIndex]++;
                return h->freq[newIndex];
            }
            newIndex = ((newIndex + step)%h->capacity);
            colisions += 1;
            i++;
        }
    }
    return 0;
}


/**
 * Runs the given function f on each of the items in the table.
 *
 * @param h the table to run the function on.
 * @param f the function to run on each item.
 */
void htable_print(htable h, void f(int x,char *str)){
    int i;
    for(i=0;i<h->capacity;i++){
        if(h->freq[i]>0){
            f(h->freq[i],h->keys[i]);
        }
    }
}

/**
 * Converts a given character pointer into an integer.
 *
 * @param h the table to search.
 * @param str the key to search for.
 *
 * @return frequency of key if found, otherwise 0.
 */
int htable_search(htable h, char *str){
    int collisions = 0;
    unsigned int result = htable_word_to_int(str);
    unsigned int i = result % h->capacity;
    unsigned int step = htable_step(h,result);
    int cap = h->capacity;
    while(h->freq[i] != 0 && strcmp(str,h->keys[i]) != 0 && collisions < cap){
        i = ((i + step)%h->capacity);
        collisions++;
    }
    if(collisions == cap){
        return 0;
    }else {
        return h->freq[i];
    }
}

/**
 * Prints out all the details about the hash table
 * such as the logical address, collisions, frequency and the node.
 * 
 * 
 * @param h the table to print.
 */
void htable_print_entire_table(htable h){
    int i = 0;
    printf("%5s %5s %5s   %s\n","LL","Stats","Freq","Word");
    printf("----------------------------\n");
    while (i<h->capacity){
        if(h->freq[i] == 0){
            printf("%5d %5d %5d   %s\n",i,h->stats[i],h->freq[i]," ");
        }else{
            printf("%5d %5d %5d   %s\n",i,h->stats[i],h->freq[i],h->keys[i]);
        }
        i++;
    }
}


/**
 * Prints out a line of data from the hash table to reflect the state
 * the table was in when it was a certain percentage full.
 * Note: If the hashtable is less full than percent_full then no data
 * will be printed.
 *
 * @param h - the hash table.
 * @param stream - a stream to print the data to.
 * @param percent_full - the point at which to show the data from.
 */
static void print_stats_line(htable h, FILE *stream, int percent_full) {
    int current_entries = h->capacity * percent_full / 100;
    double average_collisions = 0.0;
    int at_home = 0;
    int max_collisions = 0;
    int i = 0;

    if (current_entries > 0 && current_entries <= h->num_keys) {
        for (i = 0; i < current_entries; i++) {
            if (h->stats[i] == 0) {
                at_home++;
            } 
            if (h->stats[i] > max_collisions) {
                max_collisions = h->stats[i];
            }
            average_collisions += h->stats[i];
        }
    
        fprintf(stream, "%4d %10d %10.1f %10.2f %11d\n", percent_full, 
                current_entries, at_home * 100.0 / current_entries,
                average_collisions / current_entries, max_collisions);
    }
}

/**
 * Prints out a table showing what the following attributes were like
 * at regular intervals (as determined by num_stats) while the
 * hashtable was being built.
 *
 * @li Percent At Home - how many keys were placed without a collision
 * occurring.
 * @li Average Collisions - how many collisions have occurred on
 *  average while placing all of the keys so far.
 * @li Maximum Collisions - the most collisions that have occurred
 * while placing a key.
 *
 * @param h the hashtable to print statistics summary from.
 * @param stream the stream to send output to.
 * @param num_stats the maximum number of statistical snapshots to print.
 */
void htable_print_stats(htable h, FILE *stream, int num_stats) {
    int i;
    fprintf(stream, "\n%s\n\n", 
            IS_LINEAR(h)  ? "Linear Probing" : "Double Hashing"); 
    fprintf(stream, "Percent   Current   Percent    Average      Maximum\n");
    fprintf(stream, " Full     Entries   At Home   Collisions   Collisions\n");
    fprintf(stream, "-----------------------------------------------------\n");
    for (i = 1; i <= num_stats; i++) {
        print_stats_line(h, stream, 100 * i / num_stats);
    }
    fprintf(stream, "-----------------------------------------------------\n\n");
}

