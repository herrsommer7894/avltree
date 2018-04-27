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

static inline avl_tree_node_t* avl_tree_middle_travel_get_post_node(avl_tree_node_t *start, avl_tree_node_t *sentinel);

static inline avl_tree_node_t* avl_tree_find_node(avl_tree_node_t *root, int key, avl_tree_node_t *sentinel);

static inline avl_tree_node_t *bst_insert_node(avl_tree_t *tree, avl_tree_node_t *node);


/*init a avl-tree root*/
int
avl_tree_init(avl_tree_t *tree, avl_tree_node_t *sentinel)
{
	if(NULL == tree){
		return -1;
	}
	avl_tree_init_sentinel(sentinel);
	tree->sentinel = sentinel;
	tree->root = sentinel;
	return 0;
}

/* insert a key into avl-tree */
int 
avl_tree_insert_key(avl_tree_t *tree, int key)
{
	if(NULL == tree){
		return -1;
	}
	avl_tree_node_t *node = (avl_tree_node_t *)calloc(1,sizeof(avl_tree_node_t));
	if(NULL == node){
		return  -2;
	}
	node->key = key;
	node->left_child = node->right_child = tree->sentinel;
	node->height = 1;
	return avl_tree_insert_node(tree, node);
}

/* delete key from avl-tree */
int 
avl_tree_delete_key(avl_tree_t *tree, int key)
{
	if(NULL == tree){
		return -1;
	}
	avl_tree_node_t * target = avl_tree_find_node(tree->root, key, tree->sentinel);
	if(target == tree->sentinel){
		return -3;
	}
	return avl_tree_delete_node(tree, target);
}

/*insert a node into avl-tree */
int
avl_tree_insert_node(avl_tree_t *tree, avl_tree_node_t *node)
{
	if(NULL == tree){
		return -1;
	}
	avl_tree_node_t *sentinel, *temp, *root, *first_bf, *parent;

	sentinel = tree->sentinel;
	root = tree->root;
	/* avl-tree is empty and directly insert */
	if(sentinel == root){ 
		node->parent = NULL;
		node->left_child = sentinel;
		node->right_child = sentinel;
		node->height = 1;
		root = node;
		return 0;
	}
	/* insert a node using BST feature */
	first_bf = bst_insert_node(tree, node);

	/* re-balance avl-tree if necessary */	
	while(first_bf != sentinel){
		parent = first_bf->parent;

		if(avl_tree_get_height(first_bf->left_child, sentinel) > avl_tree_get_height(first_bf->right_child, sentinel)){//L
			temp = first_bf->left_child;

			if(avl_tree_get_height(temp->left_child, sentinel) > avl_tree_get_height(temp->right_child, sentinel)){//L
				/* LL type, just a step : right rotate */
				avl_tree_right_rotate(&root, first_bf, sentinel);
			}else{//R
				/* LR type, two step : firstly left ratate and right rotate */
				avl_tree_left_rotate(&root, first_bf->right_child, sentinel);
				avl_tree_right_rotate(&root, first_bf, sentinel);
			}
		}else{//R

			if(avl_tree_get_height(temp->left_child, sentinel) > avl_tree_get_height(temp->right_child, sentinel)){//L
				/* RL type, two step: firstly right rotate and left rotate */
				avl_tree_right_rotate(&root, first_bf->left_child, sentinel);
				avl_tree_left_rotate(&root, first_bf, sentinel)
			}else{//R
				/* RR type, just a  step: left rotate */
				avl_tree_left_rotate(&root, first_bf, sentinel);
			}
		}
		/* via back-track algorithm to recompute children' height and get other unbalanced node */
		first_bf = avl_tree_back_track_recompute_children_height(root, parent, sentinel);
	}

	return 0;
}

/*delete a node from avl-tree */
int
avl_tree_delete_node(avl_tree_t *tree, avl_tree_node_t *node)
{
	avl_tree_node_t *root, *sentinel, *temp, *first_bf, *parent;
	if(NULL == tree || NULL == node){
		return -1;
	}

	root = tree->root;
	sentinel = tree->sentinel;

	/* firstly deletion and recompute height , re-balance if necessary */
	if(node->left_child == sentinel && node->right_child == sentinel){	/* the would-be deleted node is leaf node */
		if(node == node->parent->left_child){
			node->parent->left_child = sentinel;
		}else{
			node->parent->right_child = sentinel;
		}
	}else if(node->left_child != sentinel && node->right_child == sentinel){	/* the would-be deleted node only have left child node */
		if(node->parent->left_child == node){
			node->parent->left_child = node->left_child;
		}else{
			node->parent->right_child = node->left_child;
		}
		node->left_child->parent = node->parent;
	}else if(node->right_child != sentinel && node->left_child == sentinel){	/* the would-be deleted node only have right child node */
		if(node->parent->left_child == node){
			node->parent->left_child = node->right_child;
		}else{
			node->parent->right_child = node->right_child;
		}
		node->right_child->parent = node->parent;
	}else{	/* the would-be deleted node both have two nodes */
		temp = avl_tree_middle_travel_get_post_node(node->right_child, sentinel);
		
		if(temp ->left_child != sentinel){
			temp = temp->left_child;
			/* delete temp */
			temp->parent->left_child = sentinel;
		}else{
			temp = temp->right_child;
			/* delete temp */
			temp->parent->right_child = sentinel;
		}
		/* exchange key */
		node->key = temp->key;
		temp->parent = NULL;
	}
	/* via back-track algorithm to recompute children' height and get other unbalanced node */
	first_bf = avl_tree_back_track_recompute_children_height(root, node->parent, sentinel);
		
	/* re-balance avl-tree if necessary */	
	while(first_bf != sentinel){
		parent = first_bf->parent;

		if(avl_tree_get_height(first_bf->left_child, sentinel) > avl_tree_get_height(first_bf->right_child, sentinel)){//L
			temp = first_bf->left_child;

			if(avl_tree_get_height(temp->left_child, sentinel) > avl_tree_get_height(temp->right_child, sentinel)){//L
				/* LL type, just a step : right rotate */
				avl_tree_right_rotate(&root, first_bf, sentinel);
			}else{//R
				/* LR type, two step : firstly left ratate and right rotate */
				avl_tree_left_rotate(&root, first_bf->right_child, sentinel);
				avl_tree_right_rotate(&root, first_bf, sentinel);
			}
		}else{//R
		
			if(avl_tree_get_height(temp->left_child, sentinel) > avl_tree_get_height(temp->right_child, sentinel)){//L
				/* RL type, two step: firstly right rotate and left rotate */
				avl_tree_right_rotate(&root, first_bf->left_child, sentinel);
				avl_tree_left_rotate(&root, first_bf, sentinel)
			}else{//R
				/* RR type, just a  step: left rotate */
				avl_tree_left_rotate(&root, first_bf, sentinel);
			}
		}
		/* via back-track algorithm to recompute children' height and get other unbalanced node */
		first_bf = avl_tree_back_track_recompute_children_height(root, parent, sentinel);
	}

	return 0;
}

/* insert a node into tree in BST way */
avl_tree_node_t*
bst_insert_node(avl_tree_t *tree, avl_tree_node_t *node)
{
	avl_tree_node_t *temp = tree->root, *parent = NULL;
	for( ; temp && temp != tree->sentinel; ){
		parent = temp;
		temp = temp->key > node->key? temp->left_child: temp->right_child;
	}
	 node->parent = parent;
	 temp = node;

	/* via back-track algorithm to recompute children' height and get other unbalanced node */
	return avl_tree_back_track_recompute_children_height(tree->root, node->parent, tree->sentinel);
}

/* left rotate */
static inline void
avl_tree_left_rotate(avl_tree_node_t **root, avl_tree_node_t *node, avl_tree_node_t *sentinel)
{
	if(sentinel == *root || sentinel == node){
		return;
	}
	avl_tree_node_t *temp;
	temp = node->right_child;

	node->right_child = temp->left_child;
	if(sentinel != temp->left_child){
		temp->left_child->parent = node;
	}

	temp->parent = node->parent;

	if(node == *root){
		*root = temp;
	}else if(node == node->parent->left_child){
		node->parent->left_child = temp;
	}else{
		node->parent->right_child = temp;
	}

	temp->left_child = node;
	node->parent = temp;

}

/* right rotate */
static inline void
avl_tree_right_rotate(avl_tree_node_t **root, avl_tree_node_t *node, avl_tree_node_t *sentinel)
{
	if(sentinel == *root || sentinel == node){
		return;
	}
	avl_tree_node_t *temp;
	temp = node->left_child;

	node->left_child = temp->right_child;
	if(sentinel != temp->right_child){
		temp->right_child->parent = node;
	}
	
	temp->parent = node->parent;

	if(node == *root){
		*root = temp;
	}else if(node == node->parent->left_child){
		node->parent->left_child = temp;
	}else{
		node->parent->right_child = temp;
	}

	temp->right_child = node;
	node->parent = temp;
}

/* get height of child tree */
static inline int
avl_tree_get_height(avl_tree_node_t *node, avl_tree_node_t *sentinel)
{
	int height = 1;
	if(sentinel == node){
		goto end;
	}
	int left_height = node->left_child ? node->left_child->height : 0; 
	int right_height = node->right_child ? node->right_child->height : 0;
	height = (left_height > right_height ? left_height : right_height) + 1;
end:
	return height;

}

/* via back-strack method to recompute height of tree */
static inline avl_tree_node_t*
avl_tree_back_track_recompute_children_height(avl_tree_node_t *root, avl_tree_node_t *current, avl_tree_node_t *sentinel)
{
	avl_tree_node_t* first_bf = sentinel;
	for(; current && current->parent != NULL ; ){
		current->height = avl_tree_get_height(current, sentinel);
		if(abs(avl_tree_get_height(current->left_child,sentinel) - avl_tree_get_height(current->right_child,sentinel)) > 1 && first_bf == sentinel){
			first_bf = current;
			break;
		}
		current = current->parent;
	}
	return first_bf;
}

static inline avl_tree_node_t*
avl_tree_middle_travel_get_post_node(avl_tree_node_t *start, avl_tree_node_t *sentinel)
{
	avl_tree_node_t *temp = start;
	for( ; temp && temp->left_child != sentinel ; ){
		temp = temp->left_child;
	}
	return temp;
}

static inline avl_tree_node_t*
vl_tree_find_node(avl_tree_node_t *root, int key, avl_tree_node_t *sentinel)
{
	avl_tree_node_t *temp = root, *ret = sentinel;

	for( ; temp && temp != sentinel; ){
		if(temp->key == key ){
			ret = temp;
			break;
		}
		temp = temp->key > key ?temp->left_child: temp->right_child;
	}
	return ret;
}