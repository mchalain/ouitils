#include <stdlib.h>

#include "log.h"
#include "list.h"

struct List_s
{
	void *entity;
	List_t *next;
	List_t *previous;
	List_t *last;
	List_t *iterator;
};

List_t *list_append(List_t *list, void *entity)
{
	List_t *entry = NULL;
	entry = calloc(1, sizeof(*entry));
	entry->entity = entity;
	entry->next = list;
	if (list == NULL)
		entry->last = entry;
	else
	{
		list->previous = entry;
		entry->last = list->last;
	}
	return entry;
}

List_t *list_insert(List_t *list, void *entity)
{
	List_t *entry = NULL;
	entry = calloc(1, sizeof(*entry));
	entry->entity = entity;
	if (list)
	{
		entry->previous = list->last;
		list->last->next = entry;
	}
	else
	{
		list = entry;
	}
	list->last = entry;
	return list;
}

List_t *list_last(List_t *list)
{
	return list->last;
}

int list_islast(List_t *list, void *entity)
{
	if (list->last->entity == entity)
		return 1;
	return 0;
}

int list_isfirst(List_t *list, void *entity)
{
	if (list->entity == entity)
		return 1;
	return 0;
}

void *list_next(List_t *list)
{
	if (list == NULL)
		return NULL;

	void *entity = NULL;
	List_t *entry = NULL;
	if (list->iterator == NULL)
		entry = list;
	else
		entry = list->iterator->next;

	if (entry)
		entity = entry->entity;
	list->iterator = entry;

	return entity;
}

void *list_previous(List_t *list)
{
	if (list == NULL)
		return NULL;

	void *entity = NULL;
	List_t *entry = NULL;
	if (list->iterator == NULL)
		entry = list->last;
	else
		entry = list->iterator->previous;

	if (entry)
		entity = entry->entity;
	list->iterator = entry;

	return entity;
}

void *list_pop(List_t *list)
{
	if (list == NULL)
		return NULL;

	void *entity = NULL;
	List_t *entry = list->last;

	if (entry == NULL)
		entry = list;
	entity = entry->entity;
	if (entry->previous)
	{
		entry->previous->next = NULL;
		list->last = entry->previous;
	}
	free(entry);

	return entity;
}

void list_destroy(List_t *list, void (*ofree)(void *))
{
	List_t *previous = NULL;
	for (List_t *it = list->last; it; it = previous)
	{
		if (ofree && it->entity)
			ofree(it->entity);
		previous = it->previous;
		previous->next = NULL;
		free(it);
	}

}
