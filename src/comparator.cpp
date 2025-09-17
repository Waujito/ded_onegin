#include <assert.h>
#include <stdio.h>

#include "comparator.h"

int filter_lines(const char *line) {
	assert (line);

	if (*line == '\n')
		return 0;

	if (*line == '\0')
		return 0;

	return 1;
}

static inline int is_letter(char c) {
	// 0-9
	if (c >= 48 && c <= 57) {
		return 1;
	}
	// A-Z
	if (c >= 64 && c <= 90) {
		return 1;
	}
	// a-z
	if (c >= 97 && c <= 122) {
		return 1;
	}

	return 0;
}

static inline int compare_chars(char c1, char c2) {
	// a-z to A-Z
	if (c1 >= 97 && c1 <= 122) {
		c1 -= 32;
	}
	// a-z to A-Z
	if (c2 >= 97 && c2 <= 122) {
		c2 -= 32;
	}

	if (c1 == c2) {
		return 0;
	} else if (c1 > c2) {
		return 1;
	} else {
		return -1;
	}
}

static inline void skip_no_letter(const char **letter) {
	assert (letter);

	while (**letter && !is_letter(**letter)) {
		(*letter)++;
	}
}

static inline void skip_no_letter_backwards(const char **letter, ssize_t *left_len) {
	assert (letter);
	assert (left_len);

	while (*left_len > 0 && !is_letter(**letter)) {
		(*letter)--, (*left_len)--;
	}
}

int backward_string_comparator(const void *a1, const void *a2) {
	assert (a1);
	assert (a2);

	const struct onegin_line *ln1 = (const struct onegin_line *)a1;
	const struct onegin_line *ln2 = (const struct onegin_line *)a2;

	ssize_t left_sz1 = (ssize_t) ln1->line_sz;
	ssize_t left_sz2 = (ssize_t) ln2->line_sz;

	if (left_sz1 <= 0 || left_sz2 <= 0) {
		return (int)(left_sz2 - left_sz1);
	}

	const char *t1 = ln1->line_ptr + left_sz1 - 1;
	const char *t2 = ln2->line_ptr + left_sz2 - 1;

	int ret = 0;

	do {
		skip_no_letter_backwards(&t1, &left_sz1);
		skip_no_letter_backwards(&t2, &left_sz2);

		if (left_sz1 <= 0 || left_sz2 <= 0) {
			break;
		}

		if ((ret = compare_chars(*t1, *t2)) != 0) {
			return ret;
		}

		t1--, t2--;
	} while (left_sz1-- > 0 && left_sz2-- > 0);

	if (left_sz1 < left_sz2) {
		return 1;
	} else if (left_sz1 > left_sz2) {
		return -1;
	}

	return 0;
}

int forward_string_comparator(const void *a1, const void *a2) {
	assert (a1);
	assert (a2);

	const struct onegin_line *ln1 = (const struct onegin_line *)a1;
	const struct onegin_line *ln2 = (const struct onegin_line *)a2;

	const char *t1 = ln1->line_ptr;
	const char *t2 = ln2->line_ptr;
	int ret = 0;

	do {
		skip_no_letter(&t1);
		skip_no_letter(&t2);

		if ((ret = compare_chars(*t1, *t2)) != 0) {
			return ret;
		}
	} while (*(t1++) != '\0' && *(t2++) != '\0');

	return 0;
}

int strings_comparator(const void *a1, const void *a2) {
	assert (a1);
	assert (a2);

	const char *t1 = *(const char *const *)a1;
	const char *t2 = *(const char *const *)a2;
	int ret = 0;

	do {
		skip_no_letter(&t1);
		skip_no_letter(&t2);

		if ((ret = compare_chars(*t1, *t2)) != 0) {
			return ret;
		}
	} while (*(t1++) != '\0' && *(t2++) != '\0');

	return 0;
}
