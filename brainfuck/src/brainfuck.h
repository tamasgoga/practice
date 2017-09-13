#ifndef GG_BRAINFUCK_SRC_BRAINFUCK_H
#define GG_BRAINFUCK_SRC_BRAINFUCK_H

/* "Nice", but not particularly fast or clever, Brainfuck interpreter (by Goga Tamas), based on:
 * http://www.muppetlabs.com/~breadbox/bf/
 * http://www.muppetlabs.com/~breadbox/bf/standards.html
 * http://stackoverflow.com/questions/2588163/implementing-brainfuck-loops-in-an-interpreter
 *
 * Sample programs from:
 * http://esoteric.sange.fi/brainfuck/bf-source/prog/
 * https://copy.sh/brainfuck/
 *
 * SPECS:
 * Cell size: sizeof(char)
 * Cell memory wraps around on the left (0 -> last), and expands infinitely on the right (bounded by memory).
 * Cells are initialized to 0.
 * Cell values behave like signed integral types in C.
 * "If a program attempts to input a value when there is no more data in the input stream",
 * the current cell's value will be EOF.
**/

#include <stddef.h>

#define INIT_CELLS_LEN (1024 * 1024)
#define INIT_STACK_LEN 1024

/* Contains all the error codes that a Brainfuck program could exit with. */
enum bferr {
	BFERR_OK,
	BFERR_CELL_ALLOC,     // cell-memory allocation failure
	BFERR_CELL_REALLOC,   // cell-memory reallocation failure
	BFERR_STACK_ALLOC,    // jump-stack allocation failure
	BFERR_STACK_REALLOC,  // jump-stack reallocation failure
	BFERR_NEED_END_LOOP,  // ']' expected
	BFERR_NEED_START_LOOP // '[' expected
};
typedef int bferr_t;


/* Double a buffer's size, through reallocation (unless the current size is 0 or the buffer is NULL).
 * New elements are initialized to 0.
 * Returns the size of the buffer after reallocation, or 0, on failure.
 * The caller is responsible for calling free() to deallocate the buffer.
**/
size_t doubleBufferSize(void** buffer, size_t currentLength, size_t typeSize);

/* Reads a file as a (char) string.
 * Caller is responsible for freeing the returned string.
**/
char* getFileContent(const char* relPath);

/* Holds everything a program needs to run (virtual machine). */
struct bfvm_t {
	char* cells;
	size_t cellsLength;

	size_t* jumpStack;
	size_t jumpStackLength;
};

/* Initialize a bfvm_t object.
 * (!) Previously allocated data in vm will be overridden.
 * Returns: BFERR_OK or BFERR_CELL_ALLOC or BFERR_STACK_ALLOC.
 * In case of error, vm will be empty.
**/
bferr_t bfvmInit(struct bfvm_t* vm);

/* Free items contained by a bfvm_t, not the bfvm_t itself! */
void bfvmFree(struct bfvm_t* vm);

/* Simplified doubleBufferSize() calls for bfvm_t */
size_t bfvmDoubleCells(struct bfvm_t* vm);
size_t bfvmDoubleJumpStack(struct bfvm_t* vm);

/* Reads an array of characters & runs it as a Brainfuck program.
 * Returns: BFERR_OK or BFERR_CELL_REALLOC or BFERR_STACK_REALLOC or BFERR_NEED_END_LOOP or BFERR_NEED_START_LOOP.
**/
bferr_t bfvmRun(struct bfvm_t* vm, const char* program);

/* Interprets an array of characters as a Brainfuck program.
 * This function cleans up its internally managed bfvm_t.
 * Returns: one of the the error codes in enum bferr.
**/
bferr_t runProgram(const char* program);

#endif // GG_BRAINFUCK_SRC_BRAINFUCK_H
