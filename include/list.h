#include <stdlib.h>

#pragma once

struct Node {
	void *content;
	struct Node *next;
};

struct Node *node_new(void *content) {
	struct Node *node = malloc(sizeof(*node));
	node->content = content;
	node->next = NULL;
	return node;
}

typedef struct {
	struct Node *head;
	size_t size;
} List;

List list_new() {
	return (List) {NULL, 0};
}

struct Node *list_get_node(List list, size_t index) {
	struct Node *node = list.head;
	for (size_t i = 0; i < index; i++) {
		node = node->next;
	}
	return node;
}

void *list_get(List list, size_t index) {
	return list_get_node(list, index)->content;
}

void list_push(List *list, void *content) {
	if (list->size == 0) {
		list->head = node_new(content);
	} else {
		struct Node *previous = list_get_node(*list, list->size-1);
		previous->next = node_new(content);
	}
	list->size++;
}

void *list_remove(List *list, size_t index) {
	struct Node *node;
	if (index == 0) {
		node = list->head;
		list->head = node->next;
	} else {
		struct Node *previous = list_get_node(*list, index-1);
		node = previous->next;
		previous->next = node->next;
	}
	void *content = node->content;
	free(node);

	list->size--;
	return content;
}

void *list_pop(List *list) {
	return list_remove(list, list->size-1);
}

void list_free_contents(List list) {
	struct Node *node = list.head;
	for (size_t i = 0; i < list.size; i++) {
		free(node->content);
		node->content = NULL;
		node = node->next;
	}
}

void list_destroy(List *list) {
	struct Node *node = list->head;
	for (size_t i = 0; i < list->size; i++) {
		struct Node *next = node->next;
		free(node);
		node = next;
	}
	list->size = 0;
	list->head = NULL;
}
