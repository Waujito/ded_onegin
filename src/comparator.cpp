#include <assert.h>

#include "comparator.h"

int filter_lines(const char *line) {
	assert (line);

	if (*line == '\n')
		return 0;

	return 1;
}

int strings_comparator(const char *a1, const char *a2) {
	assert (a1);
	assert (a2);

	do {
		if (*(const unsigned char *)a1 != *(const unsigned char *)a2) {
			return *a1 > *a2;
		}
	} while (*(a1++) != '\0' && *(a2++) != '\0');

	return 0;
}
