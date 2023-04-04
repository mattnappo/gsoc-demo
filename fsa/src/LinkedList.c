/*
 * File: LinkedList.c
 */
#include <stdio.h>
#include <stdlib.h>
#include "LinkedList.h"
#include "Set.h"

typedef struct Node* Node;
struct Node {
	void *data;
	Node next;
	Node prev;
};

struct LinkedList {
	Node first;
	Node last;
};

LinkedList new_LinkedList() {
	LinkedList this = (LinkedList)malloc(sizeof(struct LinkedList));
	this->first = this->last = NULL;
	return this;
}

static Node new_Node(void *data) {
	Node this = (Node)malloc(sizeof(struct Node));
	this->data = data;
	this->next = this->prev = NULL;
	return this;
}

void LinkedList_free(LinkedList this, bool free_data_also) {
	if (this == NULL) {
		return;
	}
	// Free the elements
	Node node = this->first;
	while (node != NULL) {
		Node next = node->next;
		if (free_data_also && node->data != NULL) {
			free(node->data);
		}
		free(node);
		node = next;
	}
	// Free the list itself
	free(this);
}

bool LinkedList_isEmpty(const LinkedList this) {
	return this->first == NULL;
}

void LinkedList_add_at_front(LinkedList this, void *data) {
	Node node = new_Node(data);
	node->next = this->first;
	if (this->first != NULL) {
		this->first->prev = node;
	}
	this->first = node;
	if (this->last == NULL) {
		this->last = node;
	}
}

void LinkedList_add_at_end(LinkedList this, void *data) {
	Node node = new_Node(data);
	node->prev = this->last;
	if (this->last != NULL) {
		this->last->next = node;
	}
	this->last = node;
	if (this->first == NULL) {
		this->first = node;
	}
}

bool LinkedList_contains(const LinkedList this, void *value, bool (*equality)(void*, void*)) {
	for (Node node=this->first; node != NULL; node=node->next) {

		if (equality(node->data, value)) {
			return true;
		}
	}
	return false;
}

void LinkedList_remove(LinkedList this, void *data) {
	for (Node node=this->first; node != NULL; node=node->next) {
		if (node->data == data) {
			if (node == this->first) {
				this->first = node->next;
			}
			if (node == this->last) {
				this->last = node->prev;
			}
			if (node->prev != NULL) {
				node->prev->next = node->next;
			}
			if (node->next != NULL) {
				node->next->prev = node->prev;
			}
			free(node);
			return;
		}
	}
}    

void* LinkedList_elementAt(LinkedList this, int index) {
	int i = 0;
	for (Node node=this->first; node != NULL; node=node->next) {
		if (i == index) {
			return node->data;
		}
		i += 1;
	}
	return NULL;
}

void* LinkedList_pop(LinkedList this) {
	void *data = LinkedList_elementAt(this, 0);
	if (data != NULL) {
		LinkedList_remove(this, data); // Removes first occurrence
	}
	return data;
}

void LinkedList_iterate(const LinkedList this, void (*func)(void *)) {
	for (Node node=this->first; node != NULL; node=node->next) {
		func(node->data);
	}
}

struct LinkedListIterator {
	Node next;
};

LinkedListIterator LinkedList_iterator(const LinkedList this) {
	LinkedListIterator iterator = (LinkedListIterator)malloc(sizeof(struct LinkedListIterator));
	iterator->next = this->first;
	return iterator;
}

bool LinkedListIterator_hasNext(const LinkedListIterator this) {
	return this->next != NULL;
}

void* LinkedListIterator_next(LinkedListIterator this) {
	if (this->next == NULL) {
		return NULL;
	} else {
		void *data = this->next->data;
		this->next = this->next->next;
		return data;
	}
}

void LinkedList_print_set(LinkedList this) {
	printf("[");
	for (Node node=this->first; node != NULL; node=node->next) {
		// printf("%s", (char*)(node->data));
        Set_print((Set)node->data);
		if (node->next != NULL) {
			printf(" ");
		}
	}
	printf("]\n");
}

#ifdef MAIN

void LinkedList_print_string_list(LinkedList list) {}

int main(int argc, char **argv) {
	LinkedList list = new_LinkedList();
	printf("new list =");
	LinkedList_print_string_list(list);

	printf("adding three elements: ");
	LinkedList_add_at_end(list, "foo");
	LinkedList_add_at_end(list, "bar");
	LinkedList_add_at_end(list, "baz");
	LinkedList_print_string_list(list);
	printf("adding Ted at front: ");
	LinkedList_add_at_front(list, "Ted");
	LinkedList_print_string_list(list);

	printf("iterating over list:\n");
	LinkedListIterator iterator = LinkedList_iterator(list);
	while (LinkedListIterator_hasNext(iterator)) {
		void *data = LinkedListIterator_next(iterator);
		char *str = (char*)data;
		printf("%s\n", str);
	}
	free(iterator);

	// Test remove in middle
	printf("removing bar from middle: ");
	LinkedList_remove(list, "bar");
	LinkedList_print_string_list(list);
	// Test remove at head
	printf("removing Ted from start: ");
	LinkedList_remove(list, "Ted");
	LinkedList_print_string_list(list);
	// Test remove in end
	printf("removing baz from end: ");
	LinkedList_remove(list, "baz");
	LinkedList_print_string_list(list);

	// Test remove only element
	void *elt = LinkedList_elementAt(list, 0);
	printf("element 0 is \"%s\"\n", (char*)elt);
	printf("removing only remaining element: ");
	LinkedList_remove(list, elt);
	LinkedList_print_string_list(list);

	printf("list isEmpty: %d\n", LinkedList_isEmpty(list));

	printf("freeing list\n");
	LinkedList_free(list, false);
}

#endif
