/********************************************************\           
 * tree-main --main program for handling the tree files.*
 *                                                      *
 * Authors:    James Douglas, Paul Souter,              *
 *             Ryan Swanepoel                           *
 *                                                      *
 * Purpose:    to build an RBT or BST from inputs and   *
 *             runs methods on the tree.                *
 *                                                      *
 * Usage:                                               *
 *      ./tree [OPTION]... <STDIN>                      *
\********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <time.h>
#include "mylib.h"
#include "tree.h"


/**
 * Method that checks what color the node is
 * then prints out the color to output.
 *
 * @param *str the string that is the node in question.
 * @param f frequency of the node.
 */
void print(char *str, int f){
    fprintf(stderr, "%d\t%s\n", f, str);
}

/**
 * Main method handles the calling of the program with command line arguments.
 *
 * @param argc number of arguments given
 * @param argv command line arguments given
 */
int main(int argc, char **argv){
    const char *optstring = "c:df:orh";
    FILE *fp;
    FILE *tdot;

    tree t = NULL;
    char option;
    tree_t method = BST;
    int c,d,f,o,unknown;
    char word[256];
    char *file;
    clock_t start,end;
    double timeBuild, timeSearch;

    /*initialisation of variables for  the switch operation */
    
    c = 0;
    d = 0;
    f = 0;
    o = 0;
      
    /* decides which cases have been called.*/
    while ((option = getopt(argc, argv, optstring)) != EOF) {
        switch (option) {
            case 'c':/*-c filename*/
                /* the file to check against the dictionary*/
                c = 1;
                fp = fopen(optarg, "r");
                break;
            case 'd':
                /* prints the depth of the tree */
                d = 1;
                break;
            case 'o':
                o = 1;
                break;
            case 'f':
                /* output a representation of the tree in dot form to the file
                   'tree-view.dot using output-dot*/
                f = 1;
                file = optarg;
                tdot = fopen(file, "w");
                break;
            case 'r':
                /*make rbt instead of bst*/
                method = RBT;
                break;
                
            default:
                /* print a help message describing how to use the program */
                printf("Usage : ./tree [OPTION]... <STDIN>\n");
                printf("\nPerform various operations using a binary");
                printf(" tree.  By default, words \n");
                printf("are read from stdin and added to the tree,");
                printf(" before being printed out\n");
                printf("alongside their frequencies to stdout.\n");
                printf("\n -c FILENAME  Check spelling of");
                printf(" words in FILENAME using words\n");
                printf("\t      read from stdin as dictionary. Print timing");
                printf("\n\t      info & unknown words to stderr");
                printf(" (ignore -d & -o)\n");
                printf(" -d\t      Only print the tree depth (ignore -o)");
                printf("\n -f FILENAME  Write DOT output to FILENAME");
                printf(" (if -o given)\n");
                printf(" -o\t      Output the tree int DOT form to file");
                printf("'tree-view.dot'\n");
                printf(" -r\t      Make the tree an RBT");
                printf(" (BST is the default)\n");
                printf("\n -h \t      Display this message\n");
                
                return EXIT_SUCCESS;
        }
    }
    
    t = tree_new(method);       /* Initialises a new tree */
    start = clock();            /* Starts the clock for tree insert */
    /* Inserts the words in to the tree and colours the root black */
    while(getword(word, sizeof word, stdin) != EOF){
        t = tree_insert(t, word);
        blackener(t);
    }
    end = clock();              /*  Stops the clock */
    timeBuild = (end-start)/(double)CLOCKS_PER_SEC; /* calculates time taken */

    /* c case enabled */
    if(c ==1){
        unknown = 0;
        start = clock();        /* Starts the clock */
        /* searches for words in the dictionary */
        while(getword(word, sizeof word, fp) != EOF){
            /* if word not found */
            if(tree_search(t,word) == 0){
                printf("%s\n",word);
                unknown++;
            }
        }
        end = clock();          /* ends the clock */
        /* calculate time taken */
        timeSearch = (end-start)/(double)CLOCKS_PER_SEC;
        
        /* print out stats for fill and search */
        printf("%s\t%s%f\n","Fill time",": ",timeBuild);
        printf("%s\t%s%f\n","Search time",": ",timeSearch);
        printf("%s\t%s%d\n","unknown words","= ",unknown);

        /* close the file stream */
        fclose(fp);
    }
    /* o case enabled */
    if(o == 1 ){
        /* build the dictionary from stdin */
        while(getword(word, sizeof word, stdin) != EOF){
            t = tree_insert(t, word);
        }
        /* f case not enabled */
        if(f==0){
            tdot = fopen("tree-view.dot", "w");
            printf("%s %s\n","Creating dot file ", "tree-view.dot");
            tree_output_dot(t, tdot);
        }else{
            /* f case enabled */
            printf("%s %s\n","Creating dot file ", file);
            tree_output_dot(t, tdot);
        }
        /* close tdot */
        fclose(tdot);
    }

    /* d case enabled */
    if(d==1){
        /* print the depth of the tree */
        fprintf(stderr, "%d\n", tree_depth(t));
    }

    /* cases:c,d,(f or o) not enabled  */
    if(c==0 && d==0 && (f == 0 || o==0)){
        /* print the preorder traversal of the tree */
        tree_preorder(t, print);
    }

    /* clean up memory used */
    t = tree_free(t);

    /* program finished successfully */
    return EXIT_SUCCESS;
        
}

