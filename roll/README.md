## DiceRoll
Command line dice throwing app written in C++. There's no quality guarantee, it was written purely for recreational & educational purposes.
Over-engineered enough to make the itch go away. On the educational side it focuses on: _conditional compilation_, _unit testing_ and _OO design in C++_.

## Compilation
`make rel` -- Basic compilation. Includes all of the intended functionality, but omits any test code.

`make all` -- Full compilation; includes tests and additional debug output.

## Usage
> #### ROLLING
> `./roll 3d20`
> 1. 3 _20-sided_ dice.
>
> `./roll 6d6 2d12 6d4 d6`
> 1. 6 _6-sided_ dice;
> 1. 2 _12-sided_ dice;
> 1. 6 _4-sided_ dice;
> 1. 1 _6-sided_ die.

> #### TESTING
> `./roll test` -- Outputs the test results. Only works if the program was compiled with _make all_.

> #### INFO
> `./roll` -- Outputs usage and program information.

## License
Distributed under the MIT License.

## TODO
- Better unit tests;
- New die type w/ Die as a base class;
- Prettier output.
