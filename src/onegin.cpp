#include <stdlib.h>
#include <stdio.h>

#include "onegin.h"
#include "argparse.h"

static const char *DEFAULT_INPUT_FILENAME	= "textonegin.txt";
static const char *DEFAULT_OUTPUT_FILENAME	= ".out.onegin_english.txt";

enum arg_options {
	OPT_HELP		= 0,
	OPT_IN_FILENAME		= 1,
	OPT_OUT_FILENAME	= 2,
};

static struct option opts[] = {
	{ "help",	OPTIONAL_ARGUMENT,	OPT_HELP	},
	{ "input",	REQUIRED_ARGUMENT,	OPT_IN_FILENAME	},
	{ "output",	REQUIRED_ARGUMENT,	OPT_OUT_FILENAME},
	{ 0,		0,		0		},
};

struct program_context {
	const char *in_filename;
	const char *out_filename;
};

static void init_program_ctx(struct program_context *ctx) {
	ctx->in_filename = DEFAULT_INPUT_FILENAME;
	ctx->out_filename = DEFAULT_OUTPUT_FILENAME;
}

static int arg_callback(struct args_context ctx) {
	printf("Option %s ", ctx.opt.name);
	if (ctx.value) {
		printf("with value %s", ctx.value);
	}
	printf("\n");

	struct program_context *pg_ctx = (struct program_context *)ctx.context;

	enum arg_options opt_val = (enum arg_options) ctx.opt.val;
	switch (opt_val) {
		case OPT_HELP:
			eprintf("Hellp!\n");
			break;
		case OPT_IN_FILENAME:
			pg_ctx->in_filename = ctx.value;
			break;
		case OPT_OUT_FILENAME:
			pg_ctx->out_filename = ctx.value;
			break;
		default:
			eprintf("Undefined argument\n");
			return -1;
	}

	return 0;
}

int main(int argc, const char *argv[]) {
	int ret = 0;

	printf("Hello, onegin!\n");

	struct program_context ctx = {0};
	init_program_ctx(&ctx);

	ret = parse_args(argc, argv, opts, arg_callback, &ctx);
	if (ret) {
		printf("What the f... error ^_^ %d\n", ret);
		return EXIT_FAILURE;
	}

	FILE *in_text_file = fopen(ctx.in_filename, "r");
	if (!in_text_file) {
		fmtperror("Cannot read file %s", ctx.in_filename);

		return EXIT_FAILURE;
	}

	FILE *out_text_file = fopen(ctx.out_filename, "w");
	if (!out_text_file) {
		fmtperror("Cannot read file %s", ctx.out_filename);

		fclose(in_text_file);
		return EXIT_FAILURE;
	}

	ret = process_text(in_text_file, out_text_file);

	fclose(in_text_file);
	fclose(out_text_file);

	return 0;
}
