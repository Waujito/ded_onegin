#include <assert.h>
#include <stdio.h>

#include "sort.h"

int pvector_bubble_sort(struct pvector *lines_arr, pv_sort_comparator comparator) {
	assert (lines_arr);
	assert (comparator);

	for (size_t i = 0; i < lines_arr->len; i++) {
		for (size_t j = 0; j <= i; j++) {
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

static int mergePvector(
	struct pvector *arr, 
	size_t left, size_t mid, size_t right, 
	pv_sort_comparator comparator) {

	assert (arr);
	assert (comparator);

	pvector merge_storage = {0};

	if (pvector_init(&merge_storage)) {
		return -1;
	}

	if (pvector_set_capacity(&merge_storage, right - left)) {
		return -1;
	}

	size_t i = left, j = mid;
	while (i < mid || j < right) {
		if (i >= mid) {
			pvector_push_back(&merge_storage, arr->arr[j++]);
		} else if (j >= right) {
			pvector_push_back(&merge_storage, arr->arr[i++]);
		} else {
			int cmp = comparator(arr->arr[i], arr->arr[j]);

			if (cmp) {
				pvector_push_back(&merge_storage, 
					arr->arr[j++]);
			} else {
				pvector_push_back(&merge_storage,
					arr->arr[i++]);
			}
		}
	}

	assert(merge_storage.len == right - left);
	for (size_t arr_idx = left, merge_idx = 0; 
		arr_idx < right; arr_idx++, merge_idx++) {
		arr->arr[arr_idx] = merge_storage.arr[merge_idx];	
	}

	pvector_destroy(&merge_storage);
	return 0;
}

static int mergeSortRecursive(
	struct pvector *arr, pv_sort_comparator comparator, 
	size_t left, size_t right) {

	assert (arr);
	assert (comparator);

	if (left + 1 >= right) {
		return 0;
	}

	size_t mid = (left + right) / 2;
	if (mergeSortRecursive(arr, comparator, left, mid)) {
		return -1;
	}

	if (mergeSortRecursive(arr, comparator, mid, right)) {
		return -1;
	}

	if (mergePvector(arr, left, mid, right, comparator)) {
		return -1;
	}

	return 0;
}


int pvector_merge_sort(pvector *lines_arr, pv_sort_comparator comparator) {
	assert (lines_arr);
	assert (comparator);

	if (lines_arr->len == 0) {
		return 0;
	}

	if (mergeSortRecursive(lines_arr, comparator, 
			0, lines_arr->len)) {
		return -1;
	}
	return 0;
}
