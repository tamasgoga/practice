A config file parser that stores identifier-value pairs in a table.
Keys must start with an alphabetic or an underscore char. They may also contain numbers.

Supported types:
- `string`, 
- `number` (double),
- `boolean`.

Strings:
- must be enclosed in double quotes, like so: "my string value";
- support the following escape sequences: **\\\\**, **\\"**, **\n**, **\t**;
- if any other character follows '\\' it gets treated as a backslash character.

Note! Escape sequences are not supported outside of strings.

Reserved keywords:
- ``true``,
- ``false``.

`GgConfig.sublime-syntax` -- Syntax highlighting for sublime text.
