#ifndef TYPES_H
#define TYPES_H

#include <stdio.h>
#include <errno.h>

enum status_codes {
	S_OK	= 0,
	S_FAIL	= -1
};

#define SUCCEEDED (__ct_error) (__ct_error != S_OK)

#define      _CT_TRY                { goto __ct_try; } __ct_try: {	\
						int __ct_error = S_OK;	\
						(void)__ct_error;
#define      _CT_CHECKED( cmd )     { if (FAILED (__ct_error = (cmd))) goto __ct_catch; }
#define      _CT_FAIL               { __ct_error = S_FAIL; goto __ct_catch; }
#define      _CT_RETRY              { __ct_error = S_OK;   goto __ct_try;   }
#define      _CT_OK                 ( SUCCEEDED (__ct_error) )
#define      _CT_CATCH              goto __ct_finally; __ct_catch:
#define      _CT_RETURN             goto __ct_finally;
#define      _CT_FINALLY            __ct_finally:
#define      _CT_ENDTRY             }

#define eprintf(...) fprintf(stderr, __VA_ARGS__)

#define _CT_STRINGIZING(x)	#x
#define _CT_STR(x)		_CT_STRINGIZING(x)
#define _CT_FILE_LINE		__FILE__ ":" _CT_STR(__LINE__)

#define log_error(fmt, ...)						\
	eprintf("An error captured in " _CT_FILE_LINE			\
	": " fmt "\n", ##__VA_ARGS__)

#define log_perror(fmt, ...)						\
	log_error(fmt ": %s", ##__VA_ARGS__, strerror(errno))

#ifdef _DEBUG
	#define log_debug(...) eprintf(__VA_ARGS__)
#else /* _DEBUG */
	#define log_debug(...) (void)0
#endif /* _DEBUG */

void _i_assert_gdb_fork(void);

#define _i_assert(condition)							\
	if (!(condition)) {							\
		eprintf("\nAn assertion %s failed!\n\n", #condition);		\
		_i_assert_gdb_fork();						\
										\
		asm ("int $3");							\
	}									\
	(void)0									\

#ifdef _DEBUG
	#define i_assert(...) _i_assert(__VA_ARGS__)
#else /* _DEBUG */
	#define i_assert(...) (void)0
#endif /* _DEBUG */

#define ct_close(fd) if ((fd) >= 0) close(fd)
#define ct_fclose(file) if ((file)) fclose(file)

#endif /* TYPES_H */
