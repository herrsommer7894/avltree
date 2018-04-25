/*
 ============================================================================
 Name        : avl_tree.c
 Author      : shengchao
 Version     :
 Copyright   : Your copyright notice
 Description : implement in C, Ansi-style
 ============================================================================
 */

#include "avl_tree.h"


static inline void avl_tree_left_rotate(avl_tree_node_t **root, avl_tree_node_t node);

static inline void avl_tree_right_rotate(avl_tree_node_t **root, avl_tree_node_t node);

static inline int avl_tree_child_height_max(avl_tree_node_t *node);

static inline void avl_tree_back_track_recompute_children_height(avl_tree_node_t *root, avl_tree_node_t *current);

/*init a avl-tree root*/
void
avl_tree_init(avl_tree_t *tree, avl_tree_node_insert_func i_func)
{
	if(NULL == tree){
		return;
	}
	tree->inset_func = ifunc
}

/*insert a node into avl-tree */
void
avl_tree_insert(avl_tree_t *tree, avl_tree_node_t *node)
{

}

/*delete a node from avl-tree */
void
avl_tree_delete(avl_tree_t *tree, avl_tree_node_t *node)
{

}

/*insert a node into tree in BST way*/
void
bst_insert_node(avl_tree_node_t *root, avl_tree_node_t *node)
{
	avl_tree_node_t *temp = root, *parent = NULL;
	for(; temp != NULL; ){
		parent = temp;
		temp = temp->key > node->key? temp->left_child: temp->right_child;
	}
	 node->parent = parent;
	 temp = node;
	 node->left_child = node->right_child = NULL;
	 node->left_height = node->right_height = 0;
}

/*RR*/
static inline void
avl_tree_left_rotate(avl_tree_node_t **root, avl_tree_node_t node)
{
	if(NULL == *root || NULL == node){
		return;
	}
	avl_tree_node_t *temp;
	temp = node->right_child;

	if(NULL != temp->left_child){
		temp->left_child->parent = node;
	}

	node->right_child = temp->left_child;

	if(node == *root){
		*root = temp;
	}else if(node == node->parent->left_child){
		node->parent->left_child = temp;
	}else{
		node->parent->right_child = temp;
	}
	temp->parent = node->parent;

	temp->left_child = node;
	node->parent = temp;

	/*back-track algorithm recompute children' height */
	avl_tree_back_track_recompute_children_height(root, node);

}

/*LL*/
static inline void
avl_tree_right_rotate(avl_tree_node_t **root, avl_tree_node_t node)
{
	if(NULL == *root || NULL == node){
		return;
	}
	avl_tree_node_t *temp;
	temp = node->left_child;

	if(NULL != temp->right_child){
		temp->right_child->parent = node;
	}
	node->left_child = temp->right_child;

	if(node == *root){
		*root = temp;
	}else if(node == node->parent->left_child){
		node->parent->left_child = temp;
	}else{
		node->parent->right_child = temp;
	}
	temp->parent = node->parent;

	temp->right_child = node;
	node->parent = temp;

	/*back-track algorithm recompute children' height */
	avl_tree_back_track_recompute_children_height(root,node);
}

static inline int
avl_tree_child_height_max(avl_tree_node_t *node)
{
	int max = 0;
	if(NULL == node){
		goto end;
	}
	int left_max = node->left_height, right_max = node->right_height;

	max = left_max > right_max? left_max: right_max;
end:
	return max;

}

static inline void
avl_tree_back_track_recompute_children_height(avl_tree_node_t *root, avl_tree_node_t *current)
{
	do{
		current->left_height = avl_tree_child_height_max(current->left_child);
		current->right_height = avl_tree_child_height_max(current->right_child);
		current = current->parent;
	}while(root != current);
	root->left_height = avl_tree_height_max(root->left_child);
	root->right_height = avl_tree_height_max(root->right_child);
}
