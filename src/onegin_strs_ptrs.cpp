/**
 * Stores lines in a array of pointers to strings
 */

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "onegin.h"
#include "comparator.h"
#include "pvector.h"

int process_text_strs_ptrs(FILE *in_file, FILE *out_file) {
	assert(in_file); 	
	assert(out_file);

	int ret = 0;

	struct pvector lines_arr = {0};
	ret = pvector_init(&lines_arr);
	if (ret) {
		perror("pvector_init");
		return -1;
	}

	ret = pvector_set_destructor(&lines_arr, free);
	if (ret) {
		pvector_destroy(&lines_arr);

		perror("pvector_set_destructor");
		return -1;
	}

	char *lineptr = NULL;
	size_t linesz = 0;

	ssize_t read_bytes = 0;
	while ((read_bytes = getline(&lineptr, &linesz, in_file)) >= 0) {
		if (!filter_lines(lineptr)) {
			continue;
		}

		if (pvector_push_back(&lines_arr, lineptr) < 0) {
			pvector_destroy(&lines_arr);
			return -1;
		}

		// Null up the getline buffer, so it will allocate the new one
		lineptr = NULL;
		linesz = 0;
	}

	free (lineptr);
	lineptr = NULL;

	if (errno) {
		perror("Text buffer error");

		pvector_destroy(&lines_arr);
		return -1;
	}

	pvector_sort(&lines_arr, strings_comparator);

	for (size_t i = 0; i < lines_arr.len; i++) {
		fprintf(out_file, "%s", (const char *)lines_arr.arr[i]);
	}
	printf("text_lines_cnt: <%zu>\n", lines_arr.len);

	pvector_destroy(&lines_arr);

	return 0;
}
