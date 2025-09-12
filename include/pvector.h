#ifndef PVECTOR_H
#define PVECTOR_H

#include <stdlib.h>

typedef void (*pvector_el_destructor_t)(void *);
struct pvector {
	void **arr;
	size_t capacity;
	size_t len;

	pvector_el_destructor_t destructor;
};

int pvector_init(struct pvector *pv);
int pvector_set_destructor(struct pvector *pv, pvector_el_destructor_t destructor);

int pvector_destroy(struct pvector *pv);

ssize_t pvector_push_back(struct pvector *pv, void *ptr);
int pvector_pop_back(struct pvector *pv);

int pvector_has(struct pvector *pv, size_t idx);
void *pvector_get(struct pvector *pv, size_t idx);


#endif /* PVECTOR_H */
