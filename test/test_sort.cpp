#include "test_config.h"

#include "pvector.h"
#include "sort.h"

static int int_comparator(const void *a1, const void *a2) {
	const int *n1 = (const int *)a1;
	const int *n2 = (const int *)a2;

	return *n1 >= *n2;
}

static void test_sorting_function(pv_sorting_function sorting_fn) {
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

	sorting_fn(&arr, int_comparator);

	for (size_t i = 0; i < arr_sz; i++) {
		ASSERT_EQ(*(int *)arr.arr[i], sorted[i]);
	}

	pvector_destroy(&arr);

}

TEST(TestSort, BubbleSort) {
	test_sorting_function(pvector_bubble_sort);
}

TEST(TestSort, MergeSort) {
	test_sorting_function(pvector_merge_sort);
}
