#ifndef SORT_H
#define SORT_H

#include "pvector.h"

typedef int (*pv_sort_comparator)(const void *, const void *);

int pvector_bubble_sort(pvector *lines_arr, pv_sort_comparator comparator);

static inline int pvector_sort(pvector *lines_arr, pv_sort_comparator comparator) {
	return pvector_bubble_sort(lines_arr, comparator);
}

#endif /* SORT_H */
