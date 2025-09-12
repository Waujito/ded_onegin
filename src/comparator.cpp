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

int strings_comparator(const void *a1, const void *a2) {
	assert (a1);
	assert (a2);

	const char *t1 = (const char *)a1;
	const char *t2 = (const char *)a2;

	do {
		if (*(const unsigned char *)a1 != *(const unsigned char *)a2) {
			return *t1 > *t2;
		}
	} while (*(t1++) != '\0' && *(t2++) != '\0');

	return 0;
}
