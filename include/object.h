#ifndef __OBJECT_H__
#define __OBJECT_H__
#include <stddef.h>
#define container_of(ptr, type, member)                                        \
  ((type *)((char *)(ptr) - offsetof(type, member)));

typedef struct IteratorVtable_s IteratorVtable_t;
typedef struct ObjectVtable_s ObjectVtable_t;
typedef struct Object_s Object_t;
typedef struct Iterator_s Iterator_t;

typedef void (*node_cleanup_fn)(void *node);

struct ObjectVtable_s {
  void (*ctor)(Object_t *obj);
  void (*dtor)(Object_t *obj, const node_cleanup_fn cb);
  unsigned int (*size_of)(const Object_t *obj);
  int (*is_empty)(const Object_t *obj);
  void (*create_iterator)(Object_t *obj, Iterator_t *iter_storage);
  void (*create_reverse_iterator)(Object_t *obj, Iterator_t *iter_storage);
};

struct Object_s {
  const ObjectVtable_t *vtable;
};

struct IteratorVtable_s {
  void (*next)(Iterator_t *iter);
  void *(*get_node)(Iterator_t *iter);
  int (*is_done)(Iterator_t *iter);
};

struct Iterator_s {
  const IteratorVtable_t *vtable;
};

void new (Object_t *self, const ObjectVtable_t *vtable);
void delete (Object_t *self, const node_cleanup_fn cb);
unsigned int size_of(const Object_t *self);
int is_empty(const Object_t *obj);
void create_iterator(Object_t *obj, Iterator_t *iter_storage);
void create_reverse_iterator(Object_t *obj, Iterator_t *iter_storage);
void iterator_next(Iterator_t *iter);
void *iterator_get_node(Iterator_t *iter);
int iterator_is_done(Iterator_t *iter);
#endif
