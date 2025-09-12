#ifndef ONEGIN_H
#define ONEGIN_H

#include <stdio.h>
#include <errno.h>
#include <string.h>

#define eprintf(...) fprintf(stderr, __VA_ARGS__)
#define fmtperror(fmt, ...) fprintf(stderr, fmt ": %s\n", ##__VA_ARGS__, strerror(errno))

int process_text(FILE *in_file, FILE *out_file);

#endif /* ONEGIN_H */
