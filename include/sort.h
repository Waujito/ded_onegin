#ifndef SORT_H
#define SORT_H

#include "pvector.h"

typedef int (*pv_sort_comparator)(const void *, const void *);

typedef int (*pv_sorting_function)(struct pvector *lines_arr, 
				   pv_sort_comparator comparator);

int pvector_bubble_sort(struct pvector *lines_arr, 
			pv_sort_comparator comparator);

int pvector_merge_sort(pvector *lines_arr,
		       pv_sort_comparator comparator);

static inline int pvector_sort(struct pvector *lines_arr, 
			       pv_sort_comparator comparator) {
	// return pvector_bubble_sort(lines_arr, comparator);
	return pvector_merge_sort(lines_arr, comparator);
}

#endif /* SORT_H */
