#ifndef ONEGIN_H
#define ONEGIN_H

#include <stdio.h>
#include <errno.h>
#include <string.h>

#define eprintf(...) fprintf(stderr, __VA_ARGS__)
#define fmtperror(fmt, ...) fprintf(stderr, fmt ": %s\n", ##__VA_ARGS__, strerror(errno))

int process_text_rectangle(FILE *in_file, FILE *out_file);
int process_text_strs_ptrs(FILE *in_file, FILE *out_file);
int process_text_singlebuf(FILE *in_file, FILE *out_file);


static inline int process_text(FILE *in_file, FILE *out_file) {
	return process_text_singlebuf(in_file, out_file);
}

#endif /* ONEGIN_H */
