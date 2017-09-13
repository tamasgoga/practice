#include "src/brainfuck.h"

#include <stdio.h>
#include <stdlib.h>


int main(int argc, char** argv) {
	if (argc != 2) {
		fprintf(stderr, "Usage: %s program_rel_path\n", argv[0]);
		return 0;
	}

	char* program = getFileContent(argv[1]);
	if (program != NULL) {
		switch (runProgram(program)) {
		case BFERR_CELL_ALLOC:
			fputs("Unable to allocate memory for the cells.\n", stderr);
			break;
		case BFERR_CELL_REALLOC:
			fputs("Unable to expand cell-memory.\n", stderr);
			break;
		case BFERR_STACK_ALLOC:
			fputs("Unable to allocate memory for the jump-stack.\n", stderr);
			break;
		case BFERR_STACK_REALLOC:
			fputs("Unable expand jump-stack.\n", stderr);
			break;
		case BFERR_NEED_END_LOOP:
			fputs("']' (end loop) expected.\n", stderr);
			break;
		case BFERR_NEED_START_LOOP:
			fputs("'[' (start loop) expected.\n", stderr);
			break;
		}

		free(program);
	} else {
		fprintf(stderr, "File \"%s\" could not be read.\n", argv[1]);
	}

	return 0;
}
