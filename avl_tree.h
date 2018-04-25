
#ifndef	_AVL_TREE_H_
#define _AVL_TREE_H_

#include <stdio.h>
#include <stdlib.h>

typedef struct avl_tree_node_s  avl_tree_node_t;
struct avl_tree_node_s{
	avl_tree_node_t	*left_child;
	avl_tree_node_t	*right_child;
	avl_tree_node_t	*parent;
	int 			key;
	int 			height;
};

typedef struct avl_tree_s avl_tree_t;
typedef avl_tree_node_t* (*avl_tree_node_insert_func)(avl_tree_t *tree, avl_tree_node_t *node);

struct avl_tree_s{
	avl_tree_node_t	*root;
	avl_tree_node_t *sentinel;
	avl_tree_node_insert_func insert_func;
};

/*  
	the type of returns is int type , 0 is OK ,others are failed 
   	fthe type of returns is point type, NULL is failed,others are OK
*/

/* init a avl-tree */
int avl_tree_init(avl_tree_t *tree, avl_tree_node_t *sentinel, avl_tree_node_insert_func i_func);

/* insert a key into avl-tree */
int avl_tree_insert_key(avl_tree_t *tree, int key);

/* insert a node into avl-tree */
int avl_tree_insert_node(avl_tree_t *tree, avl_tree_node_t *node);

/* delete key from avl-tree */
int avl_tree_delete_key(avl_tree_t *tree, int key);

/* delete a node from avl-tree */
int avl_tree_delete_node(avl_tree_t *tree, avl_tree_node_t *node);

/* insert a node into tree via BST way */
avl_tree_node_t* bst_insert_node(avl_tree_t *tree, avl_tree_node_t *node);

#endif /* _AVL_TREE_H_ */
