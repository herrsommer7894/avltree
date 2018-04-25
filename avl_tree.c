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

#define avl_tree_init_sentinel(s)	\
		(s)->parent = NULL;	\
		(s)->left_child = NULL;	\
		(s)->right_child = NULL;	\
		(s)->height = 0;	\

static inline void avl_tree_left_rotate(avl_tree_node_t **root, avl_tree_node_t *node, avl_tree_node_t *sentinel);

static inline void avl_tree_right_rotate(avl_tree_node_t **root, avl_tree_node_t *node, avl_tree_node_t *sentinel);

static inline int avl_tree_get_height(avl_tree_node_t *node, avl_tree_node_t *sentinel);

static inline avl_tree_node_t* avl_tree_back_track_recompute_children_height(avl_tree_node_t *root, avl_tree_node_t *current, avl_tree_node_t *sentinel);

static inline 
/*init a avl-tree root*/
void
avl_tree_init(avl_tree_t *tree, avl_tree_node_t *sentinel, avl_tree_node_insert_func i_func)
{
	if(NULL == tree){
		return;
	}
	avl_tree_init_sentinel(sentinel);
	tree->sentinel = sentinel;
	tree->insert_func = i_func;
	tree->root = sentinel;
}

/*insert a node into avl-tree */
void
avl_tree_insert(avl_tree_t *tree, avl_tree_node_t *node)
{
	if(NULL == tree){
		return ;
	}
	avl_tree_node_t *sentinel, *temp, *root, *first_bf;

	sentinel = tree->sentinel;
	root = tree->root;
	if(sentinel == root){// dirctly insert 
		node->parent = NULL;
		node->left_child = sentinel;
		node->right_child = sentinel;
		node->height = 1;
		root = node;
		return;
	}

	first_bf = tree->insert_func(tree, node);
	/*re-balance avl-tree*/	
	while(first_bf != sentinel){
		if(avl_tree_get_height(first_bf->left_height, sentinel) > avl_tree_get_height(first_bf->right_height, sentinel)){//L
			temp = first_bf->left_child;
			if(avl_tree_get_height(temp->left_height, sentinel) > avl_tree_get_height(temp->right_height, sentinel)){//L
				/*LL type, just a step : right rotate */
				avl_tree_right_rotate(&root, first_bf, sentinel);
			}else{//R
				/* LR type, two step : firstly left ratate and right rotate */
				avl_tree_left_rotate(&root,first_bf->right_child,sentinel)
				avl_tree_right_rotate(&root, first_bf, sentinel);
			}
		}else{//R
			if(avl_tree_get_height(temp->left_height, sentinel) > avl_tree_get_height(temp->right_height, sentinel)){//L
				/* RL type, two step: firstly right rotate and left rotate */
				avl_tree_right_rotate(&root, first_bf->left_child, sentinel);
				avl_tree_left_rotate(&root,first_bf,sentinel)
			}else{//R
				/*RR type, just a  step: left rotate */
				avl_tree_left_rotate(&root, first_bf, sentinel);
			}
		}
		first_bf = avl_tree_back_track_recompute_children_height(root, node, sentinel);
	}
}

/*delete a node from avl-tree */
void
avl_tree_delete(avl_tree_t *tree, avl_tree_node_t *node)
{

}

/*insert a node into tree in BST way*/
avl_tree_node_t*
bst_insert_node(avl_tree_t *tree, avl_tree_node_t *node)
{
	avl_tree_node_t *temp = tree->root, *parent = NULL;
	for(; temp != tree->sentinel; ){
		parent = temp;
		temp = temp->key > node->key? temp->left_child: temp->right_child;
	}
	 node->parent = parent;
	 temp = node;
	 node->left_child = node->right_child = tree->sentinel;
	 node->height = 1;

	 /*recompute children' height in back-strack way*/
	 return avl_tree_back_track_recompute_children_height(tree->root, node, tree->sentinel);
}

/*RR*/
static inline void
avl_tree_left_rotate(avl_tree_node_t **root, avl_tree_node_t node, avl_tree_node_t *sentinel)
{
	if(sentinel == *root || sentinel == node){
		return;
	}
	avl_tree_node_t *temp;
	temp = node->right_child;

	if(sentinel != temp->left_child){
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
	return avl_tree_back_track_recompute_children_height(*root, node, sentinel);
}

/*LL*/
static inline void
avl_tree_right_rotate(avl_tree_node_t **root, avl_tree_node_t node, avl_tree_node_t *sentinel)
{
	if(sentinel == *root || sentinel == node){
		return;
	}
	avl_tree_node_t *temp;
	temp = node->left_child;

	if(sentinel != temp->right_child){
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
	return avl_tree_back_track_recompute_children_height(*root, node, sentinel);
}

static inline int
avl_tree_get_height(avl_tree_node_t *node, avl_tree_node_t *sentinel)
{
	int height = 1;
	if(sentinel == node){
		goto end;
	}
	int left_height = node->left_child? node->left_child->height: 0; 
	int right_height = node->right_child? node->right_child->height: 0;
	height = (left_height > right_height? left_height: right_height) + 1;
end:
	return height;

}

static inline avl_tree_node_t*
avl_tree_back_track_recompute_children_height(avl_tree_node_t *root, avl_tree_node_t *current, avl_tree_node_t *sentinel)
{
	av_tree_node_t* first_bf = sentinel;
	int left_height = 0, right_height = 0;
	for(; current->parent != NULL ; ){
		left_height = 0,right_height = 0;
		current->height = avl_tree_get_height(current, sentinel);
		if(abs(avl_tree_get_height(current->left_child,sentinel) - avl_tree_get_height(current->right_child,sentinel)) > 1 && first_bf == sentinel){
			first_bf = current;
		}
		current = current->parent;
	}
	return first_bf;
}
