#include "test_config.h"

#include "pvector.h"
#include "sort.h"

static int int_comparator(const void *a1, const void *a2) {
	const int *n1 = (const int *)a1;
	const int *n2 = (const int *)a2;

	if (*n1 == *n2) {
		return 0;
	} else if (*n1 > *n2) {
		return 1;
	} else {
		return -1;
	}
}

static int pv_int_comparator(const void *a1, const void *a2) {
	const int *n1 = *(const int *const *)a1;
	const int *n2 = *(const int *const *)a2;

	return int_comparator(n1, n2);
}

static void test_alg_sorting_function(alg_sorting_function sorting_fn) {
	int a[]		= {1 , 3 , -1, 2 , 5 , 0 , 9 , -5, 5};
	int sorted[]	= {-5, -1, 0 , 1 , 2 , 3 , 5 , 5 , 9};

	size_t arr_sz = sizeof(a) / sizeof(int);

	int ret = sorting_fn(a, arr_sz, sizeof(a[0]), int_comparator);
	ASSERT_EQ(ret, 0);

	for (size_t i = 0; i < arr_sz; i++) {
		ASSERT_EQ(a[i], sorted[i]);
	}
}

static void test_pv_sorting_function(pv_sorting_function sorting_fn) {
	int a[]		= {1 , 3 , -1, 2 , 5 , 0 , 9 , -5, 5};
	int sorted[]	= {-5, -1, 0 , 1 , 2 , 3 , 5 , 5 , 9};

	size_t arr_sz = sizeof(a) / sizeof(int);


	int ret = 0;
	
	pvector arr = {0};
	ret = pvector_init(&arr);
	ASSERT_EQ(ret, 0);

	for (size_t i = 0; i < arr_sz; i++) {
		pvector_push_back(&arr, (void *)(a + i));
	}

	sorting_fn(&arr, pv_int_comparator);

	for (size_t i = 0; i < arr_sz; i++) {
		ASSERT_EQ(*(int *)arr.arr[i], sorted[i]);
	}

	pvector_destroy(&arr);

}

TEST(TestSort, BubbleSort) {
	test_alg_sorting_function(alg_bubble_sort);
}

TEST(TestSort, MergeSort) {
	test_alg_sorting_function(alg_merge_sort);
}

TEST(TestSort, QuickSort) {
	test_alg_sorting_function(alg_quick_sort);
}

TEST(TestSort, PVBubbleSort) {
	test_pv_sorting_function(pvector_bubble_sort);
}

TEST(TestSort, PVMergeSort) {
	test_pv_sorting_function(pvector_merge_sort);
}

TEST(TestSort, PVQuickSort) {
	test_pv_sorting_function(pvector_quick_sort);
}
