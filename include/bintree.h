#ifndef BINTREE_H
#define BINTREE_H

typedef struct BinNode_s BinNode_t;

struct BinNode_s {
  struct BinNode_s *left;
  struct BinNode_s *right;
  struct BinNode_s *parent;
};

#endif
