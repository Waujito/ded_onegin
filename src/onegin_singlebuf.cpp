/**
 * USE THIS ONEGIN REALIZATION!
 *
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

static ssize_t get_file_size(FILE *file) {
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

static int read_lines(char *text_buf, size_t buflen, struct pvector *lines_arr) {
	char *start_lineptr = text_buf;
	size_t line_sz = 1;
	char *cur_lineptr = start_lineptr;

	// left_size is read_bytes instead of read_bytes - 1 because of \0 term
	for (size_t i = 0; i < buflen; i++, cur_lineptr++, line_sz++) {
		if (*cur_lineptr == '\n' || *cur_lineptr == '\0') {
			*cur_lineptr = '\0';

			if (!filter_lines(start_lineptr)) {
				start_lineptr = cur_lineptr + 1;
				line_sz = 0;

				continue;
			}

			assert (line_sz >= 1);

			struct onegin_line o_line = {
				.line_ptr = start_lineptr,
				.line_sz = line_sz - 1,
			};

			if (pvector_push_back(lines_arr, &o_line) < 0) {
				return -1;
			}

			start_lineptr = cur_lineptr + 1;
			line_sz = 0;
		}
	}

	return 0;
}

int process_text_singlebuf(FILE *in_file, FILE *out_file) {
	assert(in_file); 
	assert(out_file);

	int ret = 0;

	ssize_t fsize = get_file_size(in_file);
	if (fsize < 0) {
		return -1;
	}

	char *text_buf = (char *)calloc((size_t) fsize + 1, sizeof (char));
	if (!text_buf) {
		perror("text arr alloc");
		return -1;
	}
	
	size_t read_bytes = fread(text_buf, sizeof (char), (size_t) fsize, in_file);
	if (ferror(in_file)) {
		perror("fread ferror");

		free(text_buf);
		return -1;
	}
	text_buf[read_bytes] = '\0';

	struct pvector lines_arr = {0};
	ret = pvector_init(&lines_arr, sizeof (struct onegin_line));
	if (ret) {
		perror("pvector_init");

		free(text_buf);
		return -1;
	}

	size_t nlines = 1;
	for (size_t i = 0; i <= read_bytes; i++) {
		if (text_buf[i] == '\n' || text_buf[i] == '\0')
			nlines++;
	}

	pvector_set_capacity(&lines_arr, nlines + 1);

	if (read_lines(text_buf, read_bytes + 1, &lines_arr)) {
		pvector_destroy(&lines_arr);
		free(text_buf);
		return -1;
	}

	printf("text_lines_cnt: <%zu>\n", lines_arr.len);

	struct pvector forward_arr = {0};
	if (pvector_clone(&lines_arr, &forward_arr)) {
		pvector_destroy(&lines_arr);
		free(text_buf);
		return -1;
	}

	struct pvector backward_arr = {0};
	if (pvector_clone(&lines_arr, &backward_arr)) {
		pvector_destroy(&forward_arr);
		pvector_destroy(&lines_arr);
		free(text_buf);
		return -1;
	}

	pvector_sort(&forward_arr, forward_string_comparator);
	pvector_sort(&backward_arr, backward_string_comparator);

	fprintf(out_file, 
		"-------BEGIN-------"	"\n"
		"-------FORWARD ONEGIN-------"
		"\n"
	);
	for (size_t i = 0; i < forward_arr.len; i++) {
		struct onegin_line *o_line = (struct onegin_line *)pvector_get(&forward_arr, i);
		fprintf(out_file, "%s\n", o_line->line_ptr);
	}
	fprintf(out_file, 
		"\n"
		"-------BACKWARD ONEGIN-------"
		"\n"
	);
	for (size_t i = 0; i < backward_arr.len; i++) {
		struct onegin_line *o_line = (struct onegin_line *)pvector_get(&backward_arr, i);
		fprintf(out_file, "%s\n", o_line->line_ptr);
	}	
	fprintf(out_file, 
		"\n"
		"-------REAL ONEGIN-------"
		"\n"
	);
	for (size_t i = 0; i < lines_arr.len; i++) {
		struct onegin_line *o_line = (struct onegin_line *)pvector_get(&lines_arr, i);
		fprintf(out_file, "%s\n", o_line->line_ptr);
	}
	fprintf(out_file, 
		"-------END-------"
	);

	pvector_destroy(&backward_arr);
	pvector_destroy(&forward_arr);
	pvector_destroy(&lines_arr);
	free(text_buf);

	return 0;
}
