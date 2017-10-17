#include "ggconfig.hpp"

#include <vector>
#include <fstream>
#include <cstdio>
#include <stdexcept>

using namespace std;


namespace {

	/* Parses a file, without any thread-safety mechanics */
	class FileParser {
	public:
		/*
		Throws invalid_argument if the file could not be opened.
		what() will return the requested file path.
		*/
		FileParser(gg::ConfigStorage& cs, const char* path);

		/* Parse the opened file */
		bool parseFile();

		/* Get the current state of the parser as a string */
		const char* getState() const noexcept;

		/* Print error to console */
		void printError(const char* msg = "OK");

	private:
		/* Linked storage */
		gg::ConfigStorage::storage_t& storage;

		/* File */
		ifstream file;

		/* State vars */
		gg::ConfigStorage::STATE state, nextState;
		/* Current char */
		char currentChar;
		/* Key/value that is being parsed */
		string currentToken;

		/* The keys that are currently being declared */
		vector<string> pendingKeys;

		/* Position info */
		size_t lineCount, linePosCount;

		/* Get the next char from the file */
		bool fetchChar();

		unsigned char getCurrentUchar() {
			return static_cast<unsigned char>(currentChar);
		}

		/* Insert current keys into the table */
		void insertPendingKeys();
		void insertPendingKeys(bool value);
		void insertPendingKeys(double value); 

		bool handleEndOfBoolLiteralCandidate(bool value);		
		bool handleIsNotBoolLiteral() {
			state = gg::ConfigStorage::STATE::LVAL;
			currentToken.append(1, currentChar);
			return true;
		}

		/* Parsing for the different states */
		bool handleInitState();
		bool handleLvalState();
		bool handleRvalState();
		bool handleStrState();
		bool handleNumState();
		bool handleTrueState();
		bool handleFalseState();
		bool handleAsgnState();
	};

} // anon namespace


namespace gg {

	ConfigStorage::ConfigStorage()
		: storage()
	{;}


	ConfigStorage::~ConfigStorage() {;}


	bool ConfigStorage::parseFile(const char* path) {
		try {
			::FileParser p(*this, path);
			return p.parseFile();
		} catch (const invalid_argument& e) {
			fprintf(stderr, "%s\n", e.what());
			return false;
		} catch(const exception& e) {
			fprintf(stderr, "Unexpected ERROR: %s\n", e.what());
			return false;
		}
	}

} // namespace gg


namespace {

	FileParser::FileParser(gg::ConfigStorage& cs, const char* path)
		: storage(cs.storage)
		, file(path)
		, state()
		, nextState()
		, currentChar()
		, currentToken()
		, pendingKeys()
		, lineCount()
		, linePosCount()
	{
		if (!file.is_open()) 
			throw invalid_argument(string(path) + " could not be opened");
	}


	bool FileParser::fetchChar() {
		if (!file.good() || (currentChar = file.get()) == EOF)
			return false;

		if (currentChar == '\n') {
			lineCount++;
			linePosCount = 0;
		} else {
			linePosCount++;
		}

		return true;
	}


	bool FileParser::parseFile() {
		state = gg::ConfigStorage::STATE::INIT;
		nextState = gg::ConfigStorage::STATE::INIT;

		lineCount = 1;
		linePosCount = 0;
		currentChar = '?';

		while (fetchChar()) {
			switch (state) {
			case gg::ConfigStorage::STATE::INIT:
				if (!handleInitState()) {
					printError("Unexpected character encountered.");
					return false;
				}
				break;
			case gg::ConfigStorage::STATE::LVAL:
				if (!handleLvalState()) {
					printError("Identifier expected.");
					return false;
				}
				break;
			case gg::ConfigStorage::STATE::RVAL:
				if (!handleRvalState()) {
					printError("Value expected.");
					return false;
				}
				break;
			case gg::ConfigStorage::STATE::STR:
				if (!handleStrState()) {
					printError("String expected.");
					return false;
				}
				break;
			case gg::ConfigStorage::STATE::NUM:
				if (!handleNumState()) {
					printError("Number expected.");
					return false;
				}
				break;
			case gg::ConfigStorage::STATE::TRUE:
				if (!handleTrueState()) {
					printError("\"true\" expected.");
					return false;
				}
				break;
			case gg::ConfigStorage::STATE::FALSE:
				if (!handleFalseState()) {
					printError("\"false\" expected.");
					return false;
				}
				break;
			case gg::ConfigStorage::STATE::ASGN:
				if (!handleAsgnState()) {
					printError("Assignment operator (\"=\") expected.");
					return false;
				}
				break;
			case gg::ConfigStorage::STATE::CMNT:
				if (currentChar == '\n') {
					state = nextState;
					nextState = gg::ConfigStorage::STATE::INIT;
				}
				break;
			}
		}

		if (state == gg::ConfigStorage::STATE::INIT) {
			return true;
		} else {
			printError("Parsing finished in an unexpected state");
			return false;
		}
	}


	void FileParser::insertPendingKeys() {
		for (const auto& k: pendingKeys)
			storage[k] = currentToken;
		pendingKeys.clear();
	}


	void FileParser::insertPendingKeys(bool value) {
		for (const auto& k: pendingKeys)
			storage[k] = value;
		pendingKeys.clear();
	}


	void FileParser::insertPendingKeys(double value) {
		for (const auto& k: pendingKeys)
			storage[k] = value;
		pendingKeys.clear();
	}


	bool FileParser::handleInitState() {
		if (!isspace(getCurrentUchar())) {
			if (isdigit(getCurrentUchar()) || currentChar == '-' || currentChar == '+' || currentChar == '.') {
				return handleNumState();
			} else if (isalpha(getCurrentUchar()) || currentChar == '_') {
				switch (currentChar) {
				case 't':
					return handleTrueState();
				case 'f':
					return handleFalseState();
				default:
					currentToken.clear();
					currentToken.append(1, currentChar);
					state = gg::ConfigStorage::STATE::LVAL;
					break;
				}
			} else if (currentChar == '#') {
				state = gg::ConfigStorage::STATE::CMNT;
				nextState = gg::ConfigStorage::STATE::INIT;
			} else {
				return false;
			}
		}

		return true;
	}


	bool FileParser::handleLvalState() {
		if (isalnum(getCurrentUchar()) || currentChar == '_') {
			currentToken.append(1, currentChar);
		} else if (isspace(getCurrentUchar())) {
			pendingKeys.push_back(currentToken);
			state = gg::ConfigStorage::STATE::ASGN;
		} else if (currentChar == '=') {
			pendingKeys.push_back(currentToken);
			state = gg::ConfigStorage::STATE::RVAL;
		} else if (currentChar == '#') {
			pendingKeys.push_back(currentToken);
			state = gg::ConfigStorage::STATE::CMNT;
			nextState = gg::ConfigStorage::STATE::ASGN;
		} else {
			return false;
		}

		return true;
	}


	bool FileParser::handleRvalState() {
		if (handleInitState()) {
			if (state == gg::ConfigStorage::STATE::CMNT)
				nextState = gg::ConfigStorage::STATE::RVAL;
		} else if (currentChar == '"') {
			state = gg::ConfigStorage::STATE::STR;
			currentToken.clear();
		} else {
			return false;
		}

		return true;
	}


	bool FileParser::handleStrState() {
		if (currentChar == '\\') {
			if (!fetchChar())
				return false;

			switch (currentChar) {
			case '\\':
			case '"':
				currentToken.append(1, currentChar);
				break;

			case 'n':
				currentToken.append(1, '\n');
				break;

			case 't':
				currentToken.append(1, '\t');
				break;

			default: {
					const char str[] = {'\\', currentChar, '\0'};
					currentToken.append(str);
				} break;
			}
		} else if (currentChar == '"') {
			insertPendingKeys();
			state = gg::ConfigStorage::STATE::INIT;
		} else {
			currentToken.append(1, currentChar);
		}

		return true;
	}


	bool FileParser::handleNumState() {
		state = gg::ConfigStorage::STATE::NUM;
		currentToken.clear();

		do {
			currentToken.append(1, currentChar);
		} while (fetchChar() && (!isspace(getCurrentUchar()) && currentChar != '#'));

		size_t pos;
		double val;

		try {
			val = stod(currentToken, &pos);
		} catch (const invalid_argument& e) {
			return false;
		} catch (const out_of_range& e) {
			return false;
		}

		if (pos != currentToken.size()) {
			return false;
		}

		insertPendingKeys(val);
		state = gg::ConfigStorage::STATE::INIT;
		return true;
	}


	// candidate
	bool FileParser::handleEndOfBoolLiteralCandidate(bool value) {
		if (file.good()) {
			char ch = file.peek();

			if (!isspace(static_cast<unsigned char>(ch))) {
				if (ch != '=') {
					state = gg::ConfigStorage::STATE::LVAL;
					return true;
				}

				fetchChar();
				return false;
			}
		}

		state = gg::ConfigStorage::STATE::INIT;
		insertPendingKeys(value);
		return true;
	}


	bool FileParser::handleTrueState() {
		state = gg::ConfigStorage::STATE::TRUE;

		currentToken = "t";
		if (!fetchChar() || currentChar != 'r') return handleIsNotBoolLiteral();
		currentToken.append(1, currentChar);
		if (!fetchChar() || currentChar != 'u') return handleIsNotBoolLiteral();
		currentToken.append(1, currentChar);
		if (!fetchChar() || currentChar != 'e') return handleIsNotBoolLiteral();
		currentToken.append(1, currentChar);

		return handleEndOfBoolLiteralCandidate(true);
	}


	bool FileParser::handleFalseState() {
		state = gg::ConfigStorage::STATE::FALSE;

		currentToken = "f";
		if (!fetchChar() || currentChar != 'a') return handleIsNotBoolLiteral();
		currentToken.append(1, currentChar);
		if (!fetchChar() || currentChar != 'l') return handleIsNotBoolLiteral();
		currentToken.append(1, currentChar);
		if (!fetchChar() || currentChar != 's') return handleIsNotBoolLiteral();
		currentToken.append(1, currentChar);
		if (!fetchChar() || currentChar != 'e') return handleIsNotBoolLiteral();
		currentToken.append(1, currentChar);

		return handleEndOfBoolLiteralCandidate(false);
	}


	bool FileParser::handleAsgnState() {
		if (!isspace(getCurrentUchar())) {
			if (currentChar == '=') {
				state = gg::ConfigStorage::STATE::RVAL;
			} else if (currentChar == '#') {
				state = gg::ConfigStorage::STATE::CMNT;
				nextState = gg::ConfigStorage::STATE::ASGN;
			} else {
				return false;
			}
		}

		return true;
	}


	const char* FileParser::getState() const noexcept {
		switch (state) {
		case gg::ConfigStorage::STATE::INIT:
			return "INIT";
		case gg::ConfigStorage::STATE::LVAL:
			return "IDENTIFIER";
		case gg::ConfigStorage::STATE::RVAL:
			return "VALUE";
		case gg::ConfigStorage::STATE::STR:
			return "STRING";
		case gg::ConfigStorage::STATE::TRUE:
			return "TRUE";
		case gg::ConfigStorage::STATE::FALSE:
			return "FALSE";
		case gg::ConfigStorage::STATE::NUM:
			return "NUMBER";
		case gg::ConfigStorage::STATE::ASGN:
			return "ASSIGN";
		case gg::ConfigStorage::STATE::CMNT:
			return "COMMENT";
		}
	}


	void FileParser::printError(const char* msg) {
		const char chstr[] = {currentChar, '\0'};
		const int chint = static_cast<int>(currentChar);
		fprintf(
			stderr,
			"ERROR: %s\n    Parsing failed at line %ld, %ld (\"%s\")\n    Last parsed character: '%s' -- char code: %d\n    Expecting: %s\n",
			msg, lineCount, linePosCount, currentToken.c_str(), (chint != EOF ? chstr : "EOF"), chint, getState()
		);
	}

} // anon namespace
