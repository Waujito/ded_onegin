#include <assert.h>

#include "comparator.h"

int filter_lines(const char *line) {
	assert (line);

	if (*line == '\n')
		return 0;

	if (*line == '\0')
		return 0;

	return 1;
}

static inline __attribute__((__always_inline__)) int is_letter(char c) {
	if (c >= 48 && c <= 57) {
		return 1;
	}
	if (c >= 64 && c <= 90) {
		return 1;
	}
	if (c >= 97 && c <= 122) {
		return 1;
	}

	return 0;
}

static inline __attribute__((__always_inline__)) int compare_chars(char c1, char c2) {
	if (c1 >= 97 && c1 <= 122) {
		c1 -= 32;
	}
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

static inline __attribute__((__always_inline__)) void skip_no_letter(const char **letter) {
	while (**letter && !is_letter(**letter)) {
		(*letter)++;
	}
}

int backward_string_comparator(const void *a1, const void *a2) {
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
