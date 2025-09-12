/**
 * Stores the whole text in one array and splits it on lines vector
 */

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "onegin.h"
#include "comparator.h"
#include "pvector.h"


static int strings_sort(pvector *lines_arr) {
	assert (lines_arr);

	for (size_t i = 0; i < lines_arr->len; i++) {
		const char *a1 = (const char *)lines_arr->arr[i];

		for (size_t j = 0; j < i; j++) {
			const char *a2 = (const char *)lines_arr->arr[j];
			int cmp = strings_comparator(a1, a2);

			if (!cmp) {
				void *tmp = lines_arr->arr[i];
				lines_arr->arr[i] = lines_arr->arr[j];
				lines_arr->arr[j] = tmp;
			}
		}
	}

	return 0;
}

int process_text_singlebuf(FILE *in_file, FILE *out_file) {
	assert(in_file); 	
	assert(out_file);

	int ret = 0;	

	ret = fseek(in_file, 0, SEEK_END);
	if (ret) {
		perror("fseek to end");
		return -1;
	}

	long fsize = ftell(in_file);
	if (fsize < 0) {
		perror("ftell");
		return -1;
	}

	ret = fseek(in_file, 0, SEEK_SET);
	if (ret) {
		perror("fseek to start");
		return -1;
	}

	char *text_stream = (char *)calloc((size_t) fsize, sizeof (char));
	if (!text_stream) {
		perror("text arr alloc");
		return -1;
	}
	
	size_t read_bytes = fread(text_stream, sizeof (char), (size_t) fsize, in_file);
	if (read_bytes != (size_t) fsize) {
		perror("fread bytes");

		free(text_stream);
		return -1;
	}

	struct pvector lines_arr = {0};
	ret = pvector_init(&lines_arr);
	if (ret) {
		perror("pvector_init");

		free(text_stream);
		return -1;
	}

	char *start_lineptr = text_stream;
	char *cur_lineptr = start_lineptr;
	size_t left_size = read_bytes;

	while (left_size) {
		if (*cur_lineptr == '\n') {
			*cur_lineptr = '\0';


			if (!filter_lines(start_lineptr)) {
				start_lineptr = cur_lineptr + 1;
				goto incr_ptrs;
			}

			if (pvector_push_back(&lines_arr, start_lineptr) < 0) {
				pvector_destroy(&lines_arr);
				free(text_stream);
				return -1;
			}

			start_lineptr = cur_lineptr + 1;
		}

incr_ptrs:
		cur_lineptr++;
		left_size--;
	}

	strings_sort(&lines_arr);

	for (size_t i = 0; i < lines_arr.len; i++) {
		fprintf(out_file, "%s\n", (const char *)lines_arr.arr[i]);
	}
	printf("text_lines_cnt: <%zu>\n", lines_arr.len);

	pvector_destroy(&lines_arr);
	free(text_stream);

	return 0;
}
