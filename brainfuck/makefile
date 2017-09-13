SRC = *.c src/*.c

release: $(SRC)
	clang-3.8 -O2 -std=c99 -Wall -Wextra -Werror -o brainfuck $(SRC)

clean:
	rm brainfuck
