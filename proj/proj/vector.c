#include "vector.h"

void vector_init(vector *v) {
	v->size = 0;
	v->items = malloc(sizeof(void *) * v->size);
}

void vector_push_back(vector *v, void *item) {
	v->size++;
	v->items = realloc(v->items, sizeof(void *) * v->size);

	v->items[v->size - 1] = item;
}

void vector_erase(vector *v, int index) {
	if (index < 0 || index >= v->size) {
		return;
	}

	int i = index + 1;

	while (i < v->size) {
		v->items[i-1] = v->items[i];
		i++;
	}

	if (i == v->size) {
		v->size--;
		v->items = realloc(v->items, sizeof(void *) * v->size);
	}
}
