#include <boost/variant.hpp>
#include <string>
#include <unordered_map>
#include <utility>


namespace gg {

	/*
	A config file parser that stores identifier-value pairs in a table.
	Keys must start with an alphabetic or an underscore char. They may also contain numbers.

	Supported types:
		- string, 
		- number (double),
		- boolean.

	Strings:
		- must be enclosed in double quotes, like so: "my string value";
		- support the following escape sequences: \\, \", \n, \t;
		- if any other character follows '\' it gets treated as a backslash character.

	Note! Escape sequences are not supported outside of strings.

	Reserved keywords:
		- true,
		- false.
	*/
	class ConfigStorage {
	public:
		/* State information (what the parser is expecting) */
		enum class STATE {
			INIT,     // initial state
			LVAL,     // reading an identifier
			RVAL,     // reading a value
			STR,      // reading a string
			NUM,
			TRUE,     // reading a boolean
			FALSE,    // reading a boolean
			ASGN,     // reading an assignment operator
			CMNT      // we're in a comment
		};

		/* Aliases */
		using value_t = boost::variant<std::string, double, bool>;
		using storage_t = std::unordered_map<std::string, value_t>;

		/* Null element (returned by operator[] if it doesn't exist) */
		static value_t null;

		/* Key-value storage */
		storage_t storage;

		/* C-tors & d-tor */
		ConfigStorage();
		virtual ~ConfigStorage();

		/** Access operator */
		value_t& operator[](const std::string& key);
		const value_t& operator[](const std::string& key) const;

		/* Parse a config file (relative path) */
		bool parseFile(const char* path);
	};

}
