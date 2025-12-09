#include "../include/bst.h"

static void bst_ctor(Object_t *self) {
  BsTree_t *tree = (BsTree_t *)self;
  tree->cmp_key = NULL;
  tree->cmp_node = NULL;
  tree->size = 0;
  tree->root = NULL;
}

static unsigned int bst_size_of(const Object_t *self) {
  BsTree_t *tree = (BsTree_t *)self;
  return (tree != NULL) ? tree->size : 0;
}

static int bst_is_empty(const Object_t *self) {
  BsTree_t *tree = (BsTree_t *)self;
  return (tree == NULL || tree->size == 0) ? 1 : 0;
}

static void _bst_cleanup_tarverse(BinNode_t *node, node_cleanup_fn cb) {
  if (node == NULL)
    return;
  if (node->right)
    _bst_cleanup_tarverse(node->right, cb);
  if (node->left)
    _bst_cleanup_tarverse(node->left, cb);
  if (cb)
    cb(node);
}

static void bst_dtor(Object_t *self, const node_cleanup_fn cb) {
  BsTree_t *tree = (BsTree_t *)self;
  _bst_cleanup_tarverse(tree->root, cb);
  tree->root = NULL;
  tree->cmp_key = NULL;
  tree->cmp_node = NULL;
  tree->size = 0;
}

static void __bst_inorder(BinNode_t *node, VisitFunc visit, void *data) {
  if (node == NULL)
    return;
  __bst_inorder(node->left, visit, data);
  visit(node, data);
  __bst_inorder(node->right, visit, data);
}

static void __bst_postorder(BinNode_t *node, VisitFunc visit, void *data) {
  if (node == NULL)
    return;
  __bst_postorder(node->left, visit, data);
  __bst_postorder(node->right, visit, data);
  visit(node, data);
}

static void __bst_preorder(BinNode_t *node, VisitFunc visit, void *data) {
  if (node == NULL)
    return;
  visit(node, data);
  __bst_preorder(node->left, visit, data);
  __bst_preorder(node->right, visit, data);
}

void bst_inorder(BsTree_t *tree, VisitFunc visit, void *data) {
  if (tree == NULL || visit == NULL)
    return;
  __bst_inorder(tree->root, visit, data);
}

void bst_postorder(BsTree_t *tree, VisitFunc visit, void *data) {
  if (tree == NULL || visit == NULL)
    return;
  __bst_postorder(tree->root, visit, data);
}

void bst_preorder(BsTree_t *tree, VisitFunc visit, void *data) {
  if (tree == NULL || visit == NULL)
    return;
  __bst_preorder(tree->root, visit, data);
}

static BinNode_t *__insert(BsTree_t *tree, BinNode_t *root, BinNode_t *parent,
                           BinNode_t *node, BstCmpNodeFunc cmp) {
  if (root == NULL) {
    tree->size++;
    node->parent = parent;
    return node;
  }
  int flag = cmp(node, root);
  if (flag > 0)
    root->right = __insert(tree, root->right, root, node, cmp);
  else if (flag < 0)
    root->left = __insert(tree, root->left, root, node, cmp);
  return root;
}

static BinNode_t *__search(BinNode_t *root, void *data, BstCmpKeyFunc cmp) {
  if (root == NULL)
    return NULL;
  int flag = cmp(data, root);
  if (flag > 0)
    return __search(root->right, data, cmp);
  else if (flag < 0)
    return __search(root->left, data, cmp);
  return root;
}

static void __bst_replace_node(BsTree_t *tree, BinNode_t *old_node,
                               BinNode_t *new_node) {
  if (old_node->parent == NULL)
    tree->root = new_node;
  else if (old_node == old_node->parent->left)
    old_node->parent->left = new_node;
  else
    old_node->parent->right = new_node;
  if (new_node != NULL)
    new_node->parent = old_node->parent;
}

static void __bst_delete_internal(BsTree_t *tree, BinNode_t *node_to_delete) {
  if (node_to_delete == NULL)
    return;
  BinNode_t *replace_node;
  if (node_to_delete->left == NULL) {
    replace_node = node_to_delete->right;
    __bst_replace_node(tree, node_to_delete, replace_node);
  } else if (node_to_delete->right == NULL) {
    replace_node = node_to_delete->left;
    __bst_replace_node(tree, node_to_delete, replace_node);
  } else {
    BinNode_t *succ = leftmost(node_to_delete->right);
    replace_node = succ;
    __bst_replace_node(tree, succ, succ->right);
    __bst_replace_node(tree, node_to_delete, succ);
    succ->left = node_to_delete->left;
    succ->left->parent = succ;
    succ->right = node_to_delete->right;
    if (succ->right)
      succ->right->parent = succ;
  }
  tree->size--;
  INIT_BST_NODE(node_to_delete);
}

void insert(BsTree_t *tree, BinNode_t *node) {
  if (tree == NULL)
    return;
  tree->root = __insert(tree, tree->root, NULL, node, tree->cmp_node);
}

void deleteNode(BsTree_t *tree, void *data) {
  if (tree == NULL || data == NULL || tree->cmp_key == NULL)
    return;
  BinNode_t *node_to_delete = search(tree, data);
  if (node_to_delete)
    __bst_delete_internal(tree, node_to_delete);
}

BinNode_t *search(BsTree_t *tree, void *data) {
  if (tree == NULL)
    return NULL;
  return __search(tree->root, data, tree->cmp_key);
}

BinNode_t *rightmost(BinNode_t *node) {
  if (node == NULL)
    return node;
  while (node->right != NULL)
    node = node->right;
  return node;
}

BinNode_t *leftmost(BinNode_t *node) {
  if (node == NULL)
    return node;
  while (node->left != NULL)
    node = node->left;
  return node;
}

BinNode_t *getSucc(BsTree_t *tree, void *data) {
  if (tree == NULL || data == NULL || tree->cmp_key == NULL)
    return NULL;
  BinNode_t *node = search(tree, data);
  if (node == NULL)
    return NULL;
  if (node->right != NULL)
    return leftmost(node->right);

  BinNode_t *parent = node->parent;
  while (parent != NULL && node == parent->right) {
    node = parent;
    parent = parent->parent;
  }
  return parent;
}

BinNode_t *getPred(BsTree_t *tree, void *data) {
  if (tree == NULL || data == NULL || tree->cmp_key == NULL)
    return NULL;
  BinNode_t *node = search(tree, data);
  if (node == NULL)
    return NULL;
  if (node->left != NULL)
    return rightmost(node->left);
  BinNode_t *parent = node->parent;
  while (parent != NULL && node == parent->left) {
    node = parent;
    parent = parent->parent;
  }
  return parent;
}

BinNode_t *getMax(BsTree_t *tree) {
  if (tree == NULL || tree->root == NULL)
    return NULL;
  return rightmost(tree->root);
}

BinNode_t *getMin(BsTree_t *tree) {
  if (tree == NULL || tree->root == NULL)
    return NULL;
  return leftmost(tree->root);
}

static void *bst_iter_get_node(Iterator_t *iter_base) {
  BstIterator_t *iter = (BstIterator_t *)iter_base;
  return (void *)iter->current_node;
}

static int bst_iter_is_done(Iterator_t *iter_base) {
  BstIterator_t *iter = (BstIterator_t *)iter_base;
  BinNode_t *node = iter->current_node;
  return (node == NULL);
}

static void bst_iter_next(Iterator_t *iter_base) {
  BstIterator_t *iter = (BstIterator_t *)iter_base;
  BinNode_t *node = iter->current_node;
  if (node == NULL)
    return;
  if (node->right != NULL) {
    iter->current_node = leftmost(node->right);
    return;
  }
  BinNode_t *p = node->parent;
  while (p != NULL && node == p->right) {
    node = p;
    p = p->parent;
  }
  iter->current_node = p;
}

static void bst_iter_prev(Iterator_t *iter_base) {
  BstIterator_t *iter = (BstIterator_t *)iter_base;
  BinNode_t *node = iter->current_node;
  if (node == NULL)
    return;
  if (node->left != NULL) {
    iter->current_node = rightmost(node->left);
    return;
  }
  BinNode_t *p = node->parent;
  while (p != NULL && node == p->left) {
    node = p;
    p = p->parent;
  }
  iter->current_node = p;
}

static const IteratorVtable_t g_bst_iterator_vtable = {
    .next = bst_iter_next,
    .get_node = bst_iter_get_node,
    .is_done = bst_iter_is_done};
static const IteratorVtable_t g_bst_reverse_iterator_vtable = {
    .next = bst_iter_prev,
    .get_node = bst_iter_get_node,
    .is_done = bst_iter_is_done};

static void bst_create_inorder_iterator(Object_t *obj,
                                        Iterator_t *iter_storage) {
  BsTree_t *tree = (BsTree_t *)obj;
  BstIterator_t *iter = (BstIterator_t *)iter_storage;
  iter->base.vtable = &g_bst_iterator_vtable;
  iter->current_node = leftmost(tree->root);
}

static void bst_create_reverse_iterator(Object_t *obj,
                                        Iterator_t *iter_storage) {
  BsTree_t *tree = (BsTree_t *)obj;
  BstIterator_t *iter = (BstIterator_t *)iter_storage;
  iter->base.vtable = &g_bst_reverse_iterator_vtable;
  iter->current_node = rightmost(tree->root);
}

const ObjectVtable_t g_bst_vtable = {
    .ctor = bst_ctor,
    .dtor = bst_dtor,
    .size_of = bst_size_of,
    .is_empty = bst_is_empty,
    .create_iterator = bst_create_inorder_iterator,
    .create_reverse_iterator = bst_create_reverse_iterator};

void Bst_erase(BsTree_t *tree, BstIterator_t *iter) {
  if (tree == NULL || iter == NULL)
    return;
  Iterator_t *iter_base = (Iterator_t *)iter;
  if (iter_base->vtable->is_done(iter_base))
    return;
  BinNode_t *node_to_delete = iter->current_node;
  iter_base->vtable->next(iter_base);
  __bst_delete_internal(tree, node_to_delete);
}

int Bst_find(BsTree_t *tree, void *key, BstIterator_t *result_iter_out) {
  if (tree == NULL || key == NULL) {
    result_iter_out->base.vtable = &g_bst_iterator_vtable; // 指向 "end"
    result_iter_out->current_node = NULL;
    return 0;
  }

  BinNode_t *node = search(tree, key);
  result_iter_out->base.vtable = &g_bst_iterator_vtable;
  result_iter_out->current_node = node; // 可能是 NULL (end) 或 node
  return (node != NULL);
}
