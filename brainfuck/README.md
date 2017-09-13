This is a **not** particularly **clever** or **fast** *Brainfuck* interpreter.

## SPECS ##

- Cell size: ```sizeof(char)```.
- Cell vector wraps around on the left (0 -> last), and expands infinitely on the right (bounded by memory).
- Cells are initialized to 0.
- Cell values behave like signed integral types in C.
- *"If a program attempts to input a value when there is no more data in the input stream"*, the current cell's value will be EOF.

## CREDITS ##
Based on (useful resources):

- http://www.muppetlabs.com/~breadbox/bf/
- http://www.muppetlabs.com/~breadbox/bf/standards.html
- http://stackoverflow.com/questions/2588163/implementing-brainfuck-loops-in-an-interpreter

Sample programs from:

 - http://esoteric.sange.fi/brainfuck/bf-source/prog/
 - https://copy.sh/brainfuck/
 - Uncredited programs by me.

