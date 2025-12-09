#ifndef __LIST_H__
#define __LIST_H__

#include "object.h"

typedef struct list_node_s list_node_t;
typedef struct List_s List_t;
typedef struct ListIterator_s ListIterator_t;

extern const ObjectVtable_t g_list_vtable;

struct ListIterator_s {
  Iterator_t base;
  list_node_t *current_node;
  list_node_t *start_node;
};

struct list_node_s {
  struct list_node_s *next;
  struct list_node_s *prev;
};

struct List_s {
  Object_t base;
  list_node_t head;
  unsigned int size;
};

static inline void List_new(List_t *list) {
  if (list == NULL)
    return;
  new (&list->base, &g_list_vtable);
}

static inline void __list_add(list_node_t *new_node, list_node_t *prev,
                              list_node_t *next) {
  next->prev = new_node;
  new_node->next = next;
  new_node->prev = prev;
  prev->next = new_node;
}

static inline void list_add(List_t *list, list_node_t *new_node) {
  __list_add(new_node, &list->head, list->head.next);
  list->size++;
}
static inline void list_add_tail(List_t *list, list_node_t *new_node) {
  __list_add(new_node, list->head.prev, &list->head);
  list->size++;
}

static inline void __list_del(list_node_t *prev, list_node_t *next) {
  prev->next = next;
  next->prev = prev;
}

static inline void INIT_LIST_NODE(list_node_t *node) {
  if (node == NULL)
    return;
  node->next = node;
  node->prev = node;
}

static inline void list_delete_node(List_t *list, list_node_t *entry) {
  __list_del(entry->prev, entry->next);
  INIT_LIST_NODE(entry);
  list->size--;
}
#endif
