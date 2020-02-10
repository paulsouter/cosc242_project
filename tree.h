/********************************************************\           
 * tree.h --   Declaration file for tree.c              *
 *                                                      *   
 * Authors:    James Douglas, Paul Souter,              *
 *             Ryan Swanepoel                           *   
 *                                                      *   
 * Purpose:    Declare each function in tree.c          *
 *                                                      *
\********************************************************/

#ifndef TREE_H_
#define TREE_H_

typedef struct tree_node *tree;
typedef enum tree_e {BST, RBT} tree_t;

extern void blackener(tree t);
extern tree tree_free(tree t);
extern void tree_inorder(tree t, void f(char *str, int f));
extern void tree_preorder(tree r, void f(char *str, int f));
extern tree tree_insert(tree t, char *str);
extern tree tree_new(tree_t tree_type);
extern int tree_search(tree t, char *str);
extern int tree_depth(tree t);
extern void tree_output_dot(tree t, FILE *out);

#endif
