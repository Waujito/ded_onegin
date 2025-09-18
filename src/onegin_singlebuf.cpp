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

static int read_file(const char *filename, char **bufptr, size_t *read_bytes_ptr) {
	assert (filename);
	assert (bufptr);
	assert (read_bytes_ptr);

	FILE *file = fopen(filename, "rb");
	if (!file) {
		perror("fopen");
		return -1;
	}

	ssize_t fsize = get_file_size(file);
	if (fsize < 0) {
		fclose(file);
		return -1;
	}

	char *text_buf = (char *)calloc((size_t) fsize + 1, sizeof(char));
	if (!text_buf) {
		perror("text arr alloc");
		fclose(file);
		return -1;
	}
	
	size_t read_bytes = fread(text_buf, sizeof(char), (size_t) fsize, file);
	if (ferror(file)) {
		perror("fread ferror");

		free(text_buf);
		fclose(file);
		return -1;
	}
	text_buf[read_bytes] = '\0';
	read_bytes++;

	*bufptr = text_buf;

	*read_bytes_ptr = read_bytes;

	fclose(file);

	return 0;
}

static int read_lines(char *text_buf, size_t buflen, struct pvector *lines_arr) {
	assert (text_buf);
	assert (lines_arr);

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

static size_t count_lines(const char *text_buf, size_t buflen) {
	assert (text_buf);

	size_t nlines = 1;
	for (size_t i = 0; i < buflen; i++) {
		if (text_buf[i] == '\n' || text_buf[i] == '\0')
			nlines++;
	}

	return nlines;
}

static int pvector_read_lines(struct pvector *text_lines, char *buf, size_t buflen) {
	assert (text_lines);
	assert (buf);

	int ret = 0;

	ret = pvector_init(text_lines, sizeof(struct onegin_line));
	if (ret) {
		perror("pvector_init");
		return -1;
	}

	size_t nlines = count_lines(buf, buflen);

	pvector_set_capacity(text_lines, nlines + 1);

	if (read_lines(buf, buflen, text_lines)) {
		pvector_destroy(text_lines);
		return -1;
	}

	return 0;
}

static int print_section_ong(FILE *out_file, const char *label, const struct pvector *onegin_text) {
	assert (out_file);
	assert (label);
	assert (onegin_text);

	fprintf(out_file, "-------%s-------\n", label);
	for (size_t i = 0; i < onegin_text->len; i++) {
		struct onegin_line *o_line = (struct onegin_line *)pvector_get(onegin_text, i);
		fprintf(out_file, "%s\n", o_line->line_ptr);
	}

	return 0;
}

int process_text_singlebuf(const char *in_filename, const char *out_filename) {
	assert (in_filename);
	assert (out_filename);

	int ret = 0;

	char *text_buf = NULL;
	size_t read_bytes = 0;

	if (read_file(in_filename, &text_buf, &read_bytes)) {
		return -1;
	}

	struct pvector lines_arr = {0};
	struct pvector forward_arr = {0};
	struct pvector backward_arr = {0};

	if ((ret = pvector_read_lines(&lines_arr, text_buf, read_bytes))) {
		goto exit;
	}

	printf("text_lines_cnt: <%zu>\n", lines_arr.len);
	
	if ((ret = pvector_clone(&forward_arr, &lines_arr))) {
		goto exit;
	}

	if ((ret = pvector_clone(&backward_arr, &lines_arr))) {
		goto exit;
	}

	if ((ret = pvector_sort(&forward_arr, forward_string_comparator))) {
		goto exit;
	}
	if ((ret = pvector_sort(&backward_arr, backward_string_comparator))) {
		goto exit;
	}

	{
		FILE *out_file = fopen(out_filename, "wb");
		if (!out_file) {
			ret = -1;
			goto exit;
		}

		print_section_ong(out_file, "FORWARD TEXT",	&forward_arr);
		print_section_ong(out_file, "BACKWARD TEXT",	&backward_arr);
		print_section_ong(out_file, "REAL TEXT",	&lines_arr);

		fclose(out_file);
	}

exit:
	pvector_destroy(&backward_arr);
	pvector_destroy(&forward_arr);
	pvector_destroy(&lines_arr);
	free(text_buf);

	return ret;
}
