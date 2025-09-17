#include <assert.h>
#include <stdio.h>

#include "pvector.h"
#include "sort.h"

#define PVECTOR_INIT_CAPACITY (128)

int pvector_init(struct pvector *pv) {
	assert (pv);

	pv->arr = NULL;
	pv->capacity = 0;
	pv->len = 0;

	pv->destructor = NULL;

	return 0;
}

int pvector_set_destructor(struct pvector *pv, pvector_el_destructor_t destructor) {
	assert(pv);

	pv->destructor = destructor;

	return 0;
}

int pvector_set_capacity(struct pvector *pv, size_t new_cap) {
	assert(pv);

	if (new_cap < pv->len) {
		return -1;
	}

	if (new_cap < PVECTOR_INIT_CAPACITY) {
		   new_cap = PVECTOR_INIT_CAPACITY;
	}

	void **new_arr = (void **)realloc(pv->arr, 
			    new_cap * sizeof (void *));
	if (!new_arr) {
		return -1;
	}

	pv->arr = new_arr;
	pv->capacity = new_cap;
	
	return 0;
}

int pvector_destroy(struct pvector *pv) {
	assert(pv);

	if (pv->destructor) {
		for (size_t i = 0; i < pv->len; i++) {
			pv->destructor(pv->arr[i]);
		}
	}

	free(pv->arr);
	pv->arr = NULL;
	pv->capacity = 0;
	pv->len = 0;

	return 0;
}

ssize_t pvector_push_back(struct pvector *pv, void *ptr) {
	assert (pv);

	if (pv->len >= pv->capacity) {
		size_t new_cap = pv->capacity * 2;
		pvector_set_capacity(pv, new_cap);
	}

	size_t idx = pv->len++;
	pv->arr[idx] = ptr;

	return (ssize_t) idx;
}

int pvector_pop_back(struct pvector *pv) {
	assert (pv);

	if (pv->len == 0) {
		return -1;
	}

	pv->len--;

	if (pv->len <= pv->capacity / 4) {
		pvector_set_capacity(pv, pv->len);
	}

	return 0;
}

int pvector_has(struct pvector *pv, size_t idx) {
	assert(pv);

	return idx < pv->len;
}

void *pvector_get(struct pvector *pv, size_t idx) {
	assert(pv);

	if (idx >= pv->len) {
		return NULL;
	}

	return pv->arr[idx];
}

int pvector_bubble_sort(struct pvector *lines_arr, alg_sorting_comparator comparator) {
	assert (lines_arr);
	assert (comparator);

	return alg_bubble_sort(lines_arr->arr, lines_arr->len, sizeof (void *), comparator);
}

int pvector_merge_sort(struct pvector *lines_arr, alg_sorting_comparator comparator) {
	assert (lines_arr);
	assert (comparator);

	return alg_merge_sort(lines_arr->arr, lines_arr->len, sizeof (void *), comparator);
}
