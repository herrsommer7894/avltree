
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

typedef avl_tree_node_t* (*avl_tree_node_insert_func)(avl_tree_t *tree, avl_tree_node_t *node);

typedef struct avl_tree_s avl_tree_t;
struct avl_tree_s{
	avl_tree_node_t	*root;
	avl_tree_node_t *sentinel;
	avl_tree_node_insert_func insert_func;
};

void avl_tree_init(avl_tree_t *tree, avl_tree_node_t *sentinel, avl_tree_node_insert_func i_func);

void avl_tree_insert(avl_tree_t *tree, avl_tree_node_t *node);// insert a node into avl-tree

void avl_tree_delete(avl_tree_t *tree, avl_tree_node_t *node);//delete a node from avl-tree

avl_tree_node_t* bst_insert_node(avl_tree_t *tree, avl_tree_node_t *node);//insert a node into tree via BST way


#endif /* _AVL_TREE_H_ */
