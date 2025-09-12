#include <assert.h>

#include "sort.h"

int pvector_bubble_sort(pvector *lines_arr, pv_sort_comparator comparator) {
	assert (lines_arr);

	for (size_t i = 0; i < lines_arr->len; i++) {
		for (size_t j = 0; j < i; j++) {
			int cmp = comparator(lines_arr->arr[i], lines_arr->arr[j]);

			if (!cmp) {
				void *tmp = lines_arr->arr[i];
				lines_arr->arr[i] = lines_arr->arr[j];
				lines_arr->arr[j] = tmp;
			}
		}
	}

	return 0;
}
