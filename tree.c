/********************************************************\           
 * tree.c --   tree program to build a Red Black Tree or*
 *             Binary Search Tree from inputs given by  *
 *             tree-main                                *
 *                                                      *   
 * Authors:    James Douglas, Paul Souter,              *
 *             Ryan Swanepoel                           *   
 *                                                      *   
 * Purpose:    To build a Red Black Tree or Binary      *
 *             Search Tree                              *
 *                                                      *
\********************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mylib.h"
#include "tree.h"

typedef enum { RED, BLACK } tree_colour;
static tree_t tree_type; 

struct tree_node {
    char *key;
    tree_colour colour;
    tree left;
    tree right;
    int frequency;
};

#define IS_BLACK(x) ((NULL == (x)) || (BLACK == (x)->colour))
#define IS_RED(x) ((NULL != (x)) && (RED == (x)->colour))

/**
 * colours the given node BLACK
 *
 * @param t the node to colour BLACK
 */
void blackener(tree t){
    if(t!=NULL && tree_type == RBT){
        t->colour = BLACK;
    }
}

/**
 * create a new tree
 * allocate memory space for the root
 * set children and key NULL
 * set frequency 0
 * set the type of the tree to the given parameter 
 * 
 * @param tree_type_given the type of tree either BST or RBT
 *
 * @return a new single node.
 */
tree tree_new(tree_t type){
    tree result = emalloc( sizeof *result);
    result->left = NULL;
    result->right = NULL;
    result->key = NULL;
    result->frequency = 0;
    result->colour = BLACK;
    tree_type = type;
    return result;
}

/**
 * rotate the tree to the left
 * right child of the root node becomes the root
 * the root becomes the left child of the new root
 * the old left node of the new root becomes the new right node of the old root
 * 
 * @param t the tree to rotate
 *
 * @return temp the new root of the tree
 */
static tree rotate_left(tree t){
    tree temp = t->right;
    t->right = temp->left;
    temp->left = t;
    return temp;
}

/**
 * rotate the tree to the right
 * left child of the root node becomes the root
 * the root becomes the new right child of the new root
 * the old right node of the new root becomes the new left node of the old root
 * 
 * @param t the tree to rotate
 *
 * @return temp the new root of the tree
 */

static tree rotate_right(tree t){
    tree temp = t->left;
    t->left= temp->right;
    temp->right = t;
    return temp;
}

/**
 * fix the tree so that it is a real RBT
 * no red node is a parent or child of another red node
 * each leaf has an equal number of black nodes from the root
 *
 * @param t the tree to fix
 *
 * @return t the fixed RBT 
 */
static tree tree_fix(tree t){
    if(IS_RED(t->left) && t->left != NULL){
        if(IS_RED(t->left->left)&& IS_RED(t->right)){
            t->left->colour = BLACK;
            t->right->colour = BLACK;
            t->colour = RED;
            return t;
        }
        if( IS_RED(t->left->left)&& IS_BLACK(t->right)){
            t->colour = RED;
            t->left->colour = BLACK;
            t = rotate_right(t);
            return t;
        }
        if (IS_RED(t->left->right)&& IS_RED(t->right)){
            t->left->colour = BLACK;
            t->right->colour = BLACK;
            t->colour = RED;
            return t;
        }
        if(IS_RED(t->left->right)&& IS_BLACK(t->right)){
            t->colour = RED;
            t->left->right->colour = BLACK;
            t->left = rotate_left(t->left);
            t = rotate_right(t);
            return t;
        }
    }
    if(IS_RED(t->right) && t->right != NULL){
        if(IS_RED(t->right->left)&& IS_RED(t->left)){
            t->left->colour = BLACK;
            t->right->colour = BLACK;
            t->colour = RED;
            return t;
        }
        if(IS_RED(t->right->left)&& IS_BLACK(t->left)){
            t->colour = RED;
            t->right->left->colour = BLACK;
            t->right = rotate_right(t->right);
            t = rotate_left(t);
            return t;
        }
        if(IS_RED(t->right->right) && IS_RED(t->left)){
            t->left->colour = BLACK;
            t->right->colour = BLACK;
            t->colour = RED;
            return t;
        }
        if(IS_RED(t->right->right)&& IS_BLACK(t->left)){
            t->colour = RED;
            t->right->colour = BLACK;
            t = rotate_left(t);
            return t;
        }
    }
    return t;
} 
/**
 * insert an item into the tree
 * if the key already exists in the tree increment frequency
 * otherwise add it to the tree, then fix the tree if its an RBT tree
 * @param t the tree to add to
 * @param str the key to try add
 *
 * @return t the tree after changes
 */
tree tree_insert(tree t, char *str){ 
    int cmp;
    if(t ==NULL){
        t = tree_new(tree_type);
    }
    if(t->key == NULL){
        t->key = emalloc(strlen(str)+1 * sizeof t-> key[0]);
        strcpy(t->key, str);
        if(tree_type == RBT){
            t->colour = RED;
        }
    }
    cmp = strcmp(t-> key, str);
    if(cmp == 0){
        t->frequency++;
    }else if(cmp > 0){
        t->left = tree_insert(t->left, str);
    }else if(cmp< 0){
        t->right = tree_insert(t-> right, str);
    }
    if(tree_type == RBT){
        t = tree_fix(t);
    }
    return t;
}

/**
 * search the tree for the given str
 * 
 * @param t the tree to search
 * @param str the key to find
 *
 * @return result 0 if not found, 1 if found
 */
int tree_search(tree t, char *str){
    int result=0;
    int cmp;
    if(t == NULL)
        return result;
    cmp = strcmp(t-> key, str);
    if(cmp == 0)
        result++;
    if(cmp>0)
        result = tree_search(t->left, str);
    if(cmp<0)
        result = tree_search(t->right, str);
    return result;
}

/**
 * pre order traversal of the tree calling the function f on each node
 * 
 * @param t the tree to traverse through
 * @param f the function to call on each node
 */
void tree_preorder(tree t, void f(char *str, int f)){
    if(t == NULL)
        return;
    f(t->key, t->frequency);
    if(t->left != NULL)
        tree_preorder(t->left, f);
    if(t->right != NULL)
        tree_preorder(t->right, f);
}
/**
 * In order traversal of the tree calling the function f on each node
 * 
 * @param t the tree to traverse through
 * @param f the function to call on each node
 */
void tree_inorder(tree t, void f(char *str, int f)){
    if(t ==NULL)
        return;
    if(t->left != NULL)
        tree_inorder(t->left, f);
    f(t-> key, t->frequency);
    if(t->right !=NULL)
        tree_inorder(t->right, f);
}

/**
 * Find the depth of the deepest path of the tree
 * 
 * @param t the tree to find the depth of
 *
 * @return result int the depth of the deepest node
 */
int tree_depth(tree t){
    int result =0;
    int leftx=0;
    int rightx= 0;
    if(t->left !=NULL){
        leftx = 1 + tree_depth(t->left);
    }
    if(t->right !=NULL){
        rightx = 1 + tree_depth(t->right);
    }
    if(rightx < leftx){
        result += leftx;
    }
    if(leftx <= rightx){
        result += rightx;
    }
    return result;
}

/**
 * Traverses the tree writing a DOT description about connections, and
 * possibly colours, to the given output stream.
 *
 * @param t the tree to output a DOT description of.
 * @param out the stream to write the DOT output to.
 */
static void tree_output_dot_aux(tree t, FILE *out) {
    if(t->key != NULL) {
        fprintf(out, "\"%s\"[label=\"{<f0>%s:%d|{<f1>|<f2>}}\"color=%s];\n",
                t->key, t->key, t->frequency,
                (RBT == tree_type && RED == t->colour) ? "red":"black");
    }
    if(t->left != NULL) {
        tree_output_dot_aux(t->left, out);
        fprintf(out, "\"%s\":f1 -> \"%s\":f0;\n", t->key, t->left->key);
    }
    if(t->right != NULL) {
        tree_output_dot_aux(t->right, out);
        fprintf(out, "\"%s\":f2 -> \"%s\":f0;\n", t->key, t->right->key);
    }
}

/**
 * Output a DOT description of this tree to the given output stream.
 * DOT is a plain text graph description language (see www.graphviz.org).
 * You can create a viewable graph with the command
 *
 *    dot -Tpdf < graphfile.dot > graphfile.pdf
 *
 * You can also use png, ps, jpg, svg... instead of pdf
 *
 * @param t the tree to output the DOT description of.
 * @param out the stream to write the DOT description to.
 */
void tree_output_dot(tree t, FILE *out) {
    fprintf(out, "digraph tree {\nnode [shape = Mrecord, penwidth = 2];\n");
    tree_output_dot_aux(t, out);
    fprintf(out, "}\n");
}
/**
 * Free the memory space used by the tree
 * 
 * @param t the tree to free
 *
 * @return t the freed tree
 */
tree tree_free(tree t){
    if(t == NULL){
        free(t);
        return t;
    }
    if(t->left != NULL)
        tree_free(t->left);
    if(t->right != NULL)
        tree_free(t->right);
    free(t->key);
    free(t);
    return t;
}
