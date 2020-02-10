/********************************************************\           
 * htable-main Main program for handling the hash       *
 *                table files.                          *
 *                                                      *   
 * Authors:    James Douglas, Paul Souter,              *
 *             Ryan Swanepoel                           *   
 *                                                      *   
 * Purpose:    to build a hash table using linear       *
 *             probing or double hashing                *
 *                                                      * 
 * Usage:                                               *   
 *      ./htable [OPTION]... <STDIN>                    *   
\********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "mylib.h"
#include "htable.h"
#include <getopt.h>
#include <string.h>
#include <time.h>

/**
 * prints the frequency and key of each item in the table
 *
 * @param freq the frequency of the key
 * @param word the word to print
 */
static void print_info(int freq, char *word) {
    printf("%-4d %s\n", freq, word);
}

/**
 *check if given number is a prime number
 *
 * @param number the number to check
 *
 * @return 1 if number is a prime, 0 if not
 */
int IsPrime(int number) {
    if (number <= 3 && number > 1) {
        return 1;
    }else if (number%2==0 || number%3==0){ 
        return 0;
    }else {
        int i;
        for (i=5; i*i<=number; i+=6) {
            if (number % i == 0 || number%(i + 2) == 0) 
                return 0;
        }
        return 1; 
    }
}

/**
 * main method handles the calling of the program with command line arguments.
 *
 * @param argc the number of arguments given
 * @param argv the arguments given in the command line when running the program.
 *
 * @return EXIT_SUCCESS the program has finished successfully.
 */
int main(int argc, char **argv){
    const char *optstring = "c:deps:t:h";
    FILE *fp;
    
    htable tab = NULL;
    char option;
    int capacity = 113;
    int c,e,p,s,unknown;
    hashing_t method = LINEAR_P;
    char word[256];
    clock_t start,end;
    double timeBuild,timeSearch;

    /*initialisation of variables for  the switch operation */
    
    c=0;
    e=0;
    p=0;
    s=10;
    unknown = 0;

    /* decides which cases have been called.*/
    while ((option = getopt(argc,argv,optstring)) != EOF){
        switch (option) {
            case 'c':/*-c filename*/
                /*ignore -p option
                  process two groups of words,
                  group 1 from stdin use as dictionary
                  group 2 from file in commandline
                  fprintf(stdout, any word NOT in dictionary) 
                */
                c=1;
                fp = fopen(optarg,"r");
                break;
                
            case 'd':/*-d*/
                /* use double hashing (linear is default)*/
                method = DOUBLE_H;
                break;
            case 'e':/*-e*/
                /*display entire contents of hash table on stderr
                  use format: "%5d %5d %5d   %s\n"
                  display index, frequency, stats and the key if it exists*/
                e=1;
                break;
            case'p':/*-p*/
                /*print stats info using functions in print-stats.txt
                  instead of printing frequencies and words*/
                p=1;
                break;
            case's':/*-s snapshots*/
                /*display upto the given number of stats snapshots
                  when given -p as an argument
                  if table is not full then fewer snapshots will be displayed,
                  snapshots with 0 entries are not shown*/
                s = atoi(optarg);
                break;
            case't':/*-t tablesize*/
                /*use the first prime >= tablesize as the size of hash table
                  assume tablesize>0 */
                capacity = atoi(optarg);
                if(capacity == 0){
                    capacity = 113;
                }else if(capacity == 1){
                    capacity = 2;
                }
                while(!IsPrime(capacity)){
                    capacity++;
                }
                break;
            default:/*OR -h*/
                /*print a help message describing how to use the program*/
                printf("%s \n\n","Usage : ./htable [OPTION]... <STDIN>");

                printf("%s%s\n","Perform various operations using a hash",
                       " table.  By default, words are");
                printf("%s%s\n","read from stdin and added to the hash",
                       " table, before being printed out");
                printf("%s\n\n","alongside their frequencies to stdout.");

                printf(" %s %s %s%s\n","-c","FILENAME ","Check spelling of",
                       " words in FILENAME using words");
                printf("              %s%s\n","from stdin as dictionary. ",
                       " Print unknown words to");
                printf("              %s%s\n","stdout, timing info & count",
                       " to stderr (ignore -p)");
                printf(" %s %s %s%s\n","-d","         ","Use double hashing",
                       " (linear probing is the default)");
                printf(" %s %s %s%s\n","-e","         ","Display entire ",
                       "contents of hash table on stderr");
                printf(" %s %s %s%s\n","-p","         ","Print stats info",
                       " instead of frequencies & words");
                printf(" %s %s %s%s\n","-s","SNAPSHOTS","Show SNAPSHOTS ",
                       "stats snapshots (if -p is used)");
                printf(" %s %s %s%s\n\n","-t","TABLESIZE","Use the first prime",
                       " >= TABLESIZE as htable size");
                printf(" %s %s %s\n","-h","         ","Display this message");

                /* program finished successfully */
                return EXIT_SUCCESS;
        }
    }
    /* regardless of cases run this section first */
    tab = htable_new(capacity,method);
    start = clock();            /* start clock */
    while(getword(word, sizeof word, stdin) != EOF){
        htable_insert(tab, word);
    }
    end = clock();              /* end clock */
    timeBuild = (end-start)/(double)CLOCKS_PER_SEC; /* time taken */
    /* if e case was enabled */
    if(e == 1){
        htable_print_entire_table(tab);
    }
    /* if c case was enabled */
    if(c==1){
        start = clock();        /* start clock */
        while(getword(word, sizeof word, fp) != EOF){
            if(htable_search(tab,word) == 0){
                printf("%s\n",word);
                unknown++;
            }
        }
        end = clock();          /* end clock */
        timeSearch = (end-start)/(double)CLOCKS_PER_SEC; /* time taken */

        /* print out details from making the table */
        printf("%s\t%s%f\n","Fill time",": ",timeBuild);
        printf("%s\t%s%f\n","Search time",": ",timeSearch);
        printf("%s\t%s%d\n","unknown words","= ",unknown);

        /* close the file */
        fclose(fp);
    }else if(p==1){ /* c==0 */
        htable_print_stats(tab,stdout,s);
    }else{   /*c==0 && p==0  */
        htable_print(tab,print_info);
    }
    
    /* free the table */
    htable_free(tab);
    
    /* program finishes successfully */
    return EXIT_SUCCESS;
}
