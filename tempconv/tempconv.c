/* Temperature measurement unit conversion, based on the example in "Programming in C" by K&R
 * (Fahrenheit, Celsius & Kelvin)
 * Made by Goga Tamás
**/

/* PLANNED FEATURES:
 * [1.3] Document file exporting in help text
 * [1.3] CSV format when exporting to file
 * Proper argument parsing
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// function specifiers
#define INLINE

// boolean
#define false  0
#define true   1

// defines related to units
#define DEG           "º"            // degree sign
#define UNITS_ABBREV  "f/F/c/C/k/K"  // unit abbreviations accepted as input

// defines related to the program itself
#define PROGRAM  "tempconv"               // executable
#define NAME     "Temperature Converter"  // the name of the program
#define VERSION  "1.2_3"                  // current version (version history at the bottom)


//--------------------------------Text defines---------------------------------
#define ABOUT_TEXT "%s %s:" \
				   "\n  Converts between temperature measurement units." \
				   "\n  Supports Kelvin, Celsius & Fahrenheit." \
				   "\n"
#define ABOUT_ARGS NAME, VERSION
// end ABOUT

#define USAGE_TEXT "Usage:" \
				   "\n  %s" \
				   "\n  %s <value> %s" \
				   "\n  %s <from> <to> <step> %s" \
				   "\n" \
				   "\n  e.g. \"%s -4.1 c\"" \
				   "\n       \"%s -10 +160 -20 F\"" \
				   "\n       \"%s 200.15 300.15 10.15 k\"" \
				   "\n"
#define USAGE_ARGS PROGRAM, \
				   PROGRAM, UNITS_ABBREV, \
				   PROGRAM, UNITS_ABBREV, \
				   PROGRAM, \
				   PROGRAM, \
				   PROGRAM
// end USAGE

#define NOTES_TEXT "Notes:" \
				   "\n  - if <step> is negative, iteration will happen in descending order (otherwise ascending)" \
				   "\n" \
				   "\nCredits:" \
				   "\n  Made by Goga Tamás in 2015-2016\n\n"
// end HELP
//-----------------------------------------------------------------------------

// output destination
FILE* output = NULL;

/* Tries to set a new output file, by opening the specified path */
void setOutputFile(const char* path);

// ERRORS
typedef enum error_t {
	ERROR_OK,
	ERROR_BAD_UNIT,
	ERROR_BAD_NUMBER,
	ERROR_ZERO_STEP,
	ERROR_BAD_ARGS,
	ERROR_FROM_NOT_LESS_THAN_TO,

	ERROR_NEVER_HERE
} error_t;

// CONVERSION
/** Celsius <=> Fahrenheit */
INLINE double celsiusToFahrenheit(double celsius);
INLINE double fahrenheitToCelsius(double fahrenheit);

/** Kelvin <=> Celsius */
INLINE double celsiusToKelvin(double celsius);
INLINE double kelvinToCelsius(double kelvin);

/** Fahrenheit <=> Kelvin */
INLINE double fahrenheitToKelvin(double fahrenheit);
INLINE double kelvinToFahrenheit(double kelvin);


/* Converts a value from one unit to the others. */
error_t convertUnit(double value, char unit) {
	// print the output based on the unit
	switch (unit) {
	case 'c':
	case 'C':
		fprintf(output, "%.2lf ºK, %.2lf ºF\n", celsiusToKelvin(value), celsiusToFahrenheit(value));
		break;
	case 'f':
	case 'F':
		fprintf(output, "%.2lf ºK, %.2lf ºC\n", fahrenheitToKelvin(value), fahrenheitToCelsius(value));
		break;
	case 'k':
	case 'K':
		fprintf(output, "%.2lf ºC, %.2lf ºF\n", kelvinToCelsius(value), kelvinToFahrenheit(value));
		break;
	default:
		return ERROR_BAD_UNIT;
	}

	return ERROR_OK;
}

/* Converts a range of values from one unit to the others.
 * If <step> is negative, iteration will happen in descending order (otherwise ascending).
**/
error_t convertUnitRange(double from, double to, double step, char unit) {
	// format string
	const char* header_format	=	"%11s %11s %11s\n";
	const char* body_format		=	"%11.2lf %11.2lf %11.2lf\n";

	// pointers to the conversion functions
	double (*convFn_1)(double);
	double (*convFn_2)(double);

	// swap <from> & <to> if needed
	if (from >= to) {
		return ERROR_FROM_NOT_LESS_THAN_TO;
	}

	switch (unit) {
	case 'c':
	case 'C':
		fprintf(output, header_format, "Celsius", "Kelvin", "Fahrenheit");
		convFn_1 = &celsiusToKelvin;
		convFn_2 = &celsiusToFahrenheit;
		break;
	case 'f':
	case 'F':
		fprintf(output, header_format, "Fahrenheit", "Kelvin", "Celsius");
		convFn_1 = &fahrenheitToKelvin;
		convFn_2 = &fahrenheitToCelsius;
		break;
	case 'k':
	case 'K':
		fprintf(output, header_format, "Kelvin", "Celsius", "Fahrenheit");
		convFn_1 = &kelvinToCelsius;
		convFn_2 = &kelvinToFahrenheit;
		break;
	default:
		return ERROR_BAD_UNIT;
	}

	// print normally
	if (step > 0) {
		for (; from <= to; from += step) {
			fprintf(output, body_format, from, convFn_1(from), convFn_2(from));
		}
	}
	// print in reverse
	else if (step < 0) {
		for (; to >= from; to += step) {
			fprintf(output, body_format, to, convFn_1(to), convFn_2(to));
		}
	}
	// step == 0
	else {
		return ERROR_ZERO_STEP;
	}

	return ERROR_OK;
}

/* Parses the command line arguments & calls the appropriate conversion function */
error_t parseArgs(int argc, char** argv) {
	char* rest; // rest of the string in strtod()
	int firstNum = 1;

	switch (argc) {
	// help
	case 1:
		fprintf(output, ABOUT_TEXT "\n" USAGE_TEXT "\n" NOTES_TEXT, ABOUT_ARGS, USAGE_ARGS);
		break;

	// one value
	case 4:
		setOutputFile(argv[1]);
		firstNum = 2;
	case 3: {
		// convert the number
		double val = strtod(argv[firstNum], &rest);
		if (strlen(rest) != 0) {
			return ERROR_BAD_NUMBER;
		}

		// get the unit
		char* unitStr = argv[firstNum + 1];
		if (strlen(unitStr) > 1) {
			return ERROR_BAD_UNIT;
		}

		// convert & output
		return convertUnit(val, unitStr[0]);
	};

	// range
	case 6:
		setOutputFile(argv[1]);
		firstNum = 2;
	case 5: {
		// convert numbers
		double from = strtod(argv[firstNum], &rest);
		if (strlen(rest) != 0) {
			return ERROR_BAD_NUMBER;
		}

		double to = strtod(argv[firstNum + 1], &rest);
		if (strlen(rest) != 0) {
			return ERROR_BAD_NUMBER;
		}

		double step = strtod(argv[firstNum + 2], &rest);
		if (strlen(rest) != 0) {
			return ERROR_BAD_NUMBER;
		}

		// <step> must not be 0
		if (step == 0.0) {
			return ERROR_ZERO_STEP;
		}

		// get the unit
		char* unitStr = argv[firstNum + 3];
		if (strlen(unitStr) > 1) {
			return ERROR_BAD_UNIT;
		}

		// do the thing
		return convertUnitRange(from, to, step, unitStr[0]);
	};

	default:
		return ERROR_BAD_ARGS;
	}

	// should never be executed but added as a fallback
	return ERROR_NEVER_HERE;
}

/* Main -- calls the argument parser & throws errors */
int main(int argc, char** argv) {
	output = stdout;
	error_t error = parseArgs(argc, argv);

	// print error message
	if (error != ERROR_OK) {
		fprintf(stderr, "ERROR: ");

		switch (error) {
		case ERROR_BAD_UNIT:
			fputs("Unsupported unit", stderr);
			break;

		case ERROR_BAD_NUMBER:
			fputs("Unable to convert arguments into numbers", stderr);
			break;

		case ERROR_ZERO_STEP:
			fputs("<step> may not be 0", stderr);
			break;

		case ERROR_BAD_ARGS:
			fputs("Unable to work with those arguments", stderr);
			break;

		case ERROR_FROM_NOT_LESS_THAN_TO:
			fputs("<from> must be less than <to>", stderr);
			break;

		case ERROR_NEVER_HERE:
			fputs("A statement that should NEVER be executed was executed... Feelsbadman...", stderr);
			break;

		default:
			fprintf(stderr, "Unknown error code %d\n", (int) error);
			break;
		}

		fprintf(stderr, "\n" USAGE_TEXT, USAGE_ARGS);
	}

	return 0;
}


/* Celsius <=> Fahrenheit */
inline double celsiusToFahrenheit(double celsius) {return (9.0/5.0) * celsius + 32.0;}
inline double fahrenheitToCelsius(double fahrenheit) {return (5.0/9.0) * (fahrenheit-32.0);}

/* Kelvin <=> Celsius */
inline double celsiusToKelvin(double celsius) {return celsius + 273.15;}
inline double kelvinToCelsius(double kelvin) {return kelvin - 273.15;}

/* Fahrenheit <=> Kelvin */
inline double fahrenheitToKelvin(double fahrenheit) {return (fahrenheit + 459.67) * 5.0/9.0;}
inline double kelvinToFahrenheit(double kelvin) {return kelvin * 9.0/5.0 - 459.67;}


/* Close a file -- used in atexit() */
static void closeOutputFile() {
	fclose(output);
}

/* Set the output file */
void setOutputFile(const char* path) {
	output = fopen(path, "a");

	if (output == NULL) {
		output = stdout;
	} else {
		atexit(closeOutputFile);
	}
}


/* VERSION HISTORY

	Version 1.2.3:
		- Removed unnecessary string arguments and functions.
		- Refactoring.

	Version 1.2.2 (06-01-2017):
		- Refactoring.
		- Error "system" changed from a global variable to returned parameters.

	(Unknown versions, i.e. 1.2beta_2)
		- Somewhere in there, I've added support for changing the output destination (i.e. stdout or
		specified file), but it's integrated into the command-line interface yet.

	Version 1.2:
		- Now strtod() instead of the "in-house" toDouble().
		- Re=factorization: errors work much better now (return values changed as a result),
		and a bunch of functions were in-lined.
		- An error occurs when <from> is greater than <to> (instead of the values being swapped).

	Version 1.1:
		- Iterating is possible both in ascending and descending order (based on the sign
		of the iterator).
		- A bunch of re-factorization was done, for better or worse (mostly for code readability).
		- Proper error messages will be displayed when the arguments are faulty.

	Version 1.0_2:
		- Both negative and positive values can be indicated by sign (e.g. both "+3.14"
		and "-3.14" are accepted).
		- Code re-factorization & preparations for future development.
		- <step> can no longer be 0.

	Version 1.0_1:
		- FIXED a bug where if <step> was negative all kinds of funky things could happen.
		- Now the program makes sure that <from> is smaller than <to> (swaps them if not),
		and that <step> is positive.


	Version 1.0:
		- Can convert a single value or a whole range as a table.

*/


// LEGACY CODE

/* Converts a string to a double.
	<success> can be:
		- 0 if the operation succeeded;
		- the position in the string where the operation failed + 1;
		- nothing, if NULL was provided as argument instead of a valid pointer.
	The returned value is guaranteed to be correct if "success" is 0.
	1.2_2: Changed (re-factored) w/o testing -- it compiles though
**/
/*double toDouble(const char* str, unsigned int* success)
{
	unsigned int i;
	double fractional = 0; // counts the numbers since the decimal separator
	double value = 0.0;

	if (success != NULL) {
		*success = 0; // will remain 0 if the operation succeeded
	}

	// check if the sign is specified
	int negative = false;
	if (str[0] == '-') {
		i = 1;
		negative = true;
	}
	else if (str[0] == '+') {
		i = 1;
	}
	else {
		i = 0;
	}

	for (; str[i] != '\0'; ++i) {
		// if the char is a number
		if (str[i] >= '0' && str[i] <= '9') {
			if (fractional == 0) {
				value = value*10 + (str[i] - '0');
			} else {
				value += (str[i] - '0') / fractional;
				fractional *= 10;
			}
		}
		// if it's a '.'
		else if (str[i] == '.') {
			if (fractional == 0) {
				fractional = 10;
			}

			// !! ERROR: more than one fractional
			else {
				if (success != NULL) {
					*success = i + 1;
				}
				return value;
			}
		}
		// !! ERROR: bad character
		else {
			if (success != NULL) {
				*success = i + 1;
			}
			return value;
		}
	}

	return negative ? -value : value;
}*/
