#include "brainfuck.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


size_t doubleBufferSize(void** buffer, size_t currentLength, size_t typeSize) {
	if (currentLength && *buffer) {
		size_t newLength = 2 * currentLength;
		void* newBuf = realloc(*buffer, newLength * typeSize);

		if (newBuf) {
			memset((newBuf + currentLength * typeSize), 0, currentLength);
			*buffer = newBuf;
			return newLength;
		}
	}

	return 0;
}

char* getFileContent(const char* relPath) {
	char* buffer = NULL;
	long length;

	FILE* input = fopen(relPath, "rb");
	if (input) {
		fseek(input, 0, SEEK_END);
		length = ftell(input);
		fseek(input, 0, SEEK_SET);

		buffer = malloc(length);
		if (buffer) {
			fread(buffer, 1, length, input);
		}
		fclose(input);
	}

	return buffer;
}

bferr_t bfvmInit(struct bfvm_t* vm) {
	vm->cells = calloc(INIT_CELLS_LEN, sizeof(char));
	if (!vm->cells) {
		return BFERR_CELL_ALLOC;
	}

	vm->jumpStack = malloc(INIT_STACK_LEN * sizeof(size_t));
	if (!vm->jumpStack) {
		free(vm->cells);
		return BFERR_STACK_ALLOC;
	}

	vm->cellsLength = INIT_CELLS_LEN;
	vm->jumpStackLength = INIT_STACK_LEN;
	return BFERR_OK;
}

size_t bfvmDoubleCells(struct bfvm_t* vm) {
	vm->cellsLength = doubleBufferSize((void**) &(vm->cells), vm->cellsLength, sizeof(vm->cells[0]));
	return vm->cellsLength;
}

size_t bfvmDoubleJumpStack(struct bfvm_t* vm) {
	vm->jumpStackLength = doubleBufferSize((void**) &(vm->jumpStack), vm->jumpStackLength, sizeof(vm->jumpStack[0]));
	return vm->jumpStackLength;
}

void bfvmFree(struct bfvm_t* vm) {
	free(vm->cells);
	vm->cellsLength = 0;

	free(vm->jumpStack);
	vm->jumpStackLength = 0;
}

bferr_t bfvmRun(struct bfvm_t* vm, const char* program) {
	size_t cp = 0; // cell-pointer
	size_t sp = 0; // stack-pointer

	for (size_t ip = 0; program[ip] != '\0'; ++ip) {
		switch (program[ip]) {
		// move pointer
		case '>':
			if (++cp >= vm->cellsLength) {
				if (!bfvmDoubleCells(vm)) {
					return BFERR_CELL_REALLOC;
				}
			}
			break;
		case '<':
			if (cp > 0) {
				--cp;
			} else {
				cp = vm->cellsLength - 1;
			}
			break;

		// arithmetic
		case '+':
			++(vm->cells[cp]);
			break;
		case '-':
			--(vm->cells[cp]);
			break;

		// i/o
		case '.':
			putchar(vm->cells[cp]);
			break;
		case ',':
			vm->cells[cp] = getchar();
			break;

		// jump forward
		case '[':
			if (vm->cells[cp]) {
				// push jump position to stack (expand stack if necessary)
				vm->jumpStack[sp++] = ip;

				if (sp >= vm->jumpStackLength) {
					if (!bfvmDoubleJumpStack(vm)) {
						return BFERR_STACK_REALLOC;
					}
				}
			} else {
				// skip to matching ']'
				size_t nest = 1;

				do {
					switch (program[++ip]) {
					case '[':
						++nest;
						break;
					case ']':
						--nest;
						break;
					case '\0':
						return BFERR_NEED_END_LOOP;
					}
				} while (nest > 0);
			}
			break;

		// jump backward
		case ']':
			if (sp > 0) {
				if (vm->cells[cp]) {
					ip = vm->jumpStack[sp - 1];
				} else {
					--sp;
				}
				break;
			} else {
				return BFERR_NEED_START_LOOP;
			}
			break;
		}
	}

	return BFERR_OK;
}

bferr_t runProgram(const char* program) {
	struct bfvm_t vm;
	bferr_t ret = bfvmInit(&vm);
	if (ret != BFERR_OK)
		return ret;

	ret = bfvmRun(&vm, program);
	bfvmFree(&vm);
	return ret;
}
