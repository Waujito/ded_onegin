/**
 * Stores the whole text in one array and splits it on lines vector
 */

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/stat.h>

#include "onegin.h"
#include "comparator.h"
#include "pvector.h"

ssize_t get_file_size(FILE *file) {
#ifndef FGETSIZE_USE_FSEEK
	struct stat file_stat = {0};
	int ret = fstat(fileno(file), &file_stat);
	if (ret) {
		perror("fstat");
		return -1;
	}

	return file_stat.st_size;
#else /* FGETSIZE_USE_FSEEK */
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

	return fsize;
#endif /* FGETSIZE_USE_FSEEK */
}

int process_text_singlebuf(FILE *in_file, FILE *out_file) {
	assert(in_file); 
	assert(out_file);

	int ret = 0;

	ssize_t fsize = get_file_size(in_file);
	if (fsize < 0) {
		return -1;
	}

	char *text_stream = (char *)calloc((size_t) fsize, sizeof (char));
	if (!text_stream) {
		perror("text arr alloc");
		return -1;
	}
	
	size_t read_bytes = fread(text_stream, sizeof (char), (size_t) fsize, in_file);
	if (ferror(in_file)) {
		perror("fread ferror");

		free(text_stream);
		return -1;
	}

	struct pvector lines_arr = {0};
	ret = pvector_init(&lines_arr, sizeof (void *));
	if (ret) {
		perror("pvector_init");

		free(text_stream);
		return -1;
	}

	size_t nlines = 1;
	for (size_t i = 0; i < read_bytes; i++) {
		if (text_stream[i] == '\n')
			nlines++;
	}

	pvector_set_capacity(&lines_arr, nlines + 1);

	char *start_lineptr = text_stream;
	char *cur_lineptr = start_lineptr;
	size_t left_size = read_bytes;

	while (left_size) {
		if (*cur_lineptr == '\n') {
			*cur_lineptr = '\0';


			if (!filter_lines(start_lineptr)) {
				start_lineptr = cur_lineptr + 1;
				cur_lineptr++;
				left_size--;
				continue;
			}

			if (pvector_push_back(&lines_arr, &start_lineptr) < 0) {
				pvector_destroy(&lines_arr);
				free(text_stream);
				return -1;
			}

			start_lineptr = cur_lineptr + 1;
		}

		cur_lineptr++;
		left_size--;
	}

	pvector_sort(&lines_arr, strings_comparator);

	for (size_t i = 0; i < lines_arr.len; i++) {
		fprintf(out_file, "%s\n", *(const char **)pvector_get(&lines_arr, i));
	}
	printf("text_lines_cnt: <%zu>\n", lines_arr.len);

	pvector_destroy(&lines_arr);
	free(text_stream);

	return 0;
}
