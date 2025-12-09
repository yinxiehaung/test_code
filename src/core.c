#include "../include/object.h"

void new(Object_t *self, const ObjectVtable_t *vtable) {
  if (!self || !vtable)
    return;
  self->vtable = vtable;
  if (self->vtable->ctor) {
    self->vtable->ctor(self);
  }
}

void delete(Object_t *self, const node_cleanup_fn cb) {
  if (!self)
    return;
  if (self->vtable && self->vtable->dtor)
    self->vtable->dtor(self, cb);
}

unsigned int size_of(const Object_t *self) {
  if (!self)
    return 0;
  if (self->vtable && self->vtable->size_of)
    return self->vtable->size_of(self);
  return 0;
}

int is_empty(const Object_t *self) {
  if (!self)
    return 0;
  if (self->vtable && self->vtable->is_empty)
    return self->vtable->is_empty(self);
  return 0;
}

void iterator_next(Iterator_t *iter) {
  if (!iter)
    return;
  if (iter->vtable && iter->vtable->next)
    iter->vtable->next(iter);
}

void *iterator_get_node(Iterator_t *iter) {
  if (!iter)
    return iter;
  if (iter->vtable && iter->vtable->get_node)
    return iter->vtable->get_node(iter);
  return iter;
}

int iterator_is_done(Iterator_t *iter) {
  if (!iter)
    return 0;
  if (iter->vtable && iter->vtable->is_done)
    return iter->vtable->is_done(iter);
  return 0;
}

void create_iterator(Object_t *obj, Iterator_t *iter_storage) {
  if (obj && obj->vtable && obj->vtable->create_iterator) {
    obj->vtable->create_iterator(obj, iter_storage);
  }
}
void create_reverse_iterator(Object_t *obj, Iterator_t *iter_storage) {
  if (obj && obj->vtable && obj->vtable->create_reverse_iterator) {
    obj->vtable->create_reverse_iterator(obj, iter_storage);
  }
}
