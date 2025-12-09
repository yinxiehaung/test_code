#include "../include/list.h"

static void list_ctor(Object_t *self) {
  List_t *list = (List_t *)self;
  INIT_LIST_NODE(&list->head);
  list->size = 0;
}

static void list_dtor(Object_t *self, node_cleanup_fn cb) {
  List_t *list = (List_t *)self;
  list_node_t *pos, *tmp;
  pos = list->head.next;
  tmp = pos->next;
  while (pos != &list->head) {
    __list_del(pos->prev, pos->next);
    INIT_LIST_NODE(pos);
    if (cb)
      cb((void *)pos);
    pos = tmp;
    tmp = pos->next;
  }
  INIT_LIST_NODE(&list->head);
  list->size = 0;
}

static unsigned int list_size(const Object_t *self) {
  List_t *list = (List_t *)self;
  return (list != NULL) ? list->size : 0;
}

static int list_is_empty(const Object_t *self) {
  List_t *list = (List_t *)self;
  return (list != NULL && list->size == 0);
}

static void *list_iter_get_node(Iterator_t *iter) {
  ListIterator_t *list_iter = (ListIterator_t *)iter;
  return (void *)list_iter->current_node;
}

static void list_iter_next(Iterator_t *iter) {
  ListIterator_t *list_iter = (ListIterator_t *)iter;
  list_iter->current_node = list_iter->current_node->next;
}

static void list_iter_prev(Iterator_t *iter) {
  ListIterator_t *list_iter = (ListIterator_t *)iter;
  list_iter->current_node = list_iter->current_node->prev;
}

static int list_iter_is_done(Iterator_t *iter) {
  ListIterator_t *list_iter = (ListIterator_t *)iter;
  return (list_iter->start_node == list_iter->current_node);
}

static IteratorVtable_t g_list_iter_vtable = {.get_node = list_iter_get_node,
                                              .is_done = list_iter_is_done,
                                              .next = list_iter_next};

static IteratorVtable_t g_list_rev_iter_vtable = {.get_node =
                                                      list_iter_get_node,
                                                  .is_done = list_iter_is_done,
                                                  .next = list_iter_prev};

static void list_create_iter(Object_t *self, Iterator_t *iter_out) {
  List_t *list = (List_t *)self;
  ListIterator_t *iter = (ListIterator_t *)iter_out;
  iter->base.vtable = &g_list_iter_vtable;
  iter->current_node = list->head.next;
  iter->start_node = &list->head;
}

static void list_create_rev_iter(Object_t *self, Iterator_t *iter_out) {
  List_t *list = (List_t *)self;
  ListIterator_t *iter = (ListIterator_t *)iter_out;
  iter->base.vtable = &g_list_rev_iter_vtable;
  iter->current_node = list->head.prev;
  iter->start_node = &list->head;
}

const ObjectVtable_t g_list_vtable = {.ctor = list_ctor,
                                      .dtor = list_dtor,
                                      .size_of = list_size,
                                      .is_empty = list_is_empty,
                                      .create_iterator = list_create_iter,
                                      .create_reverse_iterator =
                                          list_create_rev_iter};
