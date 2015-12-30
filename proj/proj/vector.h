#ifndef __VECTOR_H
#define __VECTOR_H

typedef struct vector {
	int size;
	void** items;
} vector;

void vector_init(vector *v);
void vector_push_back(vector *v, void *item);
void vector_erase(vector *v, int index);



#endif
