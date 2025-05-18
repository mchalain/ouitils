#ifndef __OUITILS_LIST_H__
#define __OUITILS_LIST_H__

typedef struct List_s List_t;

List_t *list_append(List_t *list, void *entity);
List_t *list_insert(List_t *list, void *entity);
List_t *list_last(List_t *list);
int list_islast(List_t *list, void *entity);
int list_isfirst(List_t *list, void *entity);
void *list_next(List_t *list);
void *list_previous(List_t *list);
void *list_pop(List_t *list);
void list_destroy(List_t *list, void (*ofree)(void *));

#endif
