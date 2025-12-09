#ifndef __BST_H__
#define __BST_H__
#include "bintree.h"
#include "object.h"

typedef struct BsTree_s BsTree_t;
typedef struct BstIterator_s BstIterator_t;
typedef int (*BstCmpNodeFunc)(const BinNode_t *a, const BinNode_t *b);
typedef int (*BstCmpKeyFunc)(const void *key, const BinNode_t *node);
typedef void (*VisitFunc)(BinNode_t *node, void *data);
extern const ObjectVtable_t g_bst_vtable;

struct BstIterator_s {
  Iterator_t base;
  BinNode_t *current_node;
};

struct BsTree_s {
  Object_t base;
  BinNode_t *root;
  BstCmpNodeFunc cmp_node;
  BstCmpKeyFunc cmp_key;
  unsigned int size;
};

void bst_insert(BsTree_t *tree, BinNode_t *node);
void bst_deleteNode(BsTree_t *tree, void *data);
BinNode_t *search(BsTree_t *tree, void *data);
BinNode_t *rightmost(BinNode_t *node);
BinNode_t *leftmost(BinNode_t *node);
BinNode_t *getPred(BsTree_t *tree, void *data);
BinNode_t *getSucc(BsTree_t *tree, void *data);
BinNode_t *getMax(BsTree_t *tree);
BinNode_t *getMin(BsTree_t *tree);
void bst_erase(BsTree_t *tree, BstIterator_t *itor);
int bst_find(BsTree_t *tree, void *key, BstIterator_t *result_iter_out);
void bst_inorder(BsTree_t *tree, VisitFunc visit, void *data);
void bst_preorder(BsTree_t *tree, VisitFunc visit, void *data);
void bst_postorder(BsTree_t *tree, VisitFunc visit, void *data);

static inline void Bst_new(BsTree_t *tree, BstCmpNodeFunc cmp_node,
                           BstCmpKeyFunc cmp_key) {
  new (&tree->base, &g_bst_vtable);
  tree->cmp_node = cmp_node;
  tree->cmp_key = cmp_key;
}

static inline void INIT_BST_NODE(BinNode_t *node) {
  node->left = (BinNode_t *)NULL;
  node->right = (BinNode_t *)NULL;
  node->parent = (BinNode_t *)NULL;
}
#endif
