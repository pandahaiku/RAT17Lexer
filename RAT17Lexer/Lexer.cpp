#include "Lexer.h"

// initialize lists
string Keywords[12] = { "integer", "boolean", "floating", "if",
"fi",      "else",    "return",   "write",
"read",    "while",   "true",     "false" };
char Operators[10] = { '=', '/=', '>', '<', '=>', '<=', '+', '-', '*', '/' };
char Separators[16] = { ' ', '\t', '\n', '{', '}', '[',  ']', '(',
')', '.',  ',',  ';', ':', '\'', '@', '\"' };
char Spaces[3] = { ' ', '\t', '\n' };

// construct finite state machine
void Lexer::ConstructTable() {
	vector<int> tableRow(4);

	// Row 0
	tableRow[0] = 2;
	tableRow[1] = -1;
	tableRow[2] = 1;
	tableRow[3] = -1;
	Table.push_back(tableRow);

	// Row 1
	tableRow[0] = -1;
	tableRow[1] = -1;
	tableRow[2] = 1;
	tableRow[3] = 5;
	Table.push_back(tableRow);

	// Row 2
	tableRow[0] = 2;
	tableRow[1] = 3;
	tableRow[2] = -1;
	tableRow[3] = -1;
	Table.push_back(tableRow);

	// Row 3
	tableRow[0] = 4;
	tableRow[1] = -1;
	tableRow[2] = -1;
	tableRow[3] = -1;
	Table.push_back(tableRow);

	// Row 4
	tableRow[0] = 4;
	tableRow[1] = 3;
	tableRow[2] = -1;
	tableRow[3] = -1;
	Table.push_back(tableRow);

	// Row 5
	tableRow[0] = -1;
	tableRow[1] = -1;
	tableRow[2] = 6;
	tableRow[3] = -1;
	Table.push_back(tableRow);

	// Row 6
	tableRow[0] = -1;
	tableRow[1] = -1;
	tableRow[2] = 6;
	tableRow[3] = -1;
	Table.push_back(tableRow);
}

// takes a txt file from user input //
void Lexer::GetTextFile(ifstream &file) {
	cout << "Enter the Destination of the File.\nExample: "
		"C:\\Users\\Name\\Desktop\\file.txt\n"
		<< endl;
	cout << "Please Enter File Destination: ";

	fileLocation = "c:\\users\\Douglas\\desktop\\file.txt";
	// getline(cin, fileLocation);

	file.open(fileLocation, ios::in);
	while (!file.is_open()) {
		cout << "\n\nThe file you specified could not be found. Please try again."
			<< endl;
		file.close();
		cout << "Example: C:\\Users\\Name\\Desktop\\file.txt\n" << endl;
		cout << "Please Enter File Destination: ";
		getline(cin, fileLocation);
		file.open(fileLocation, ios::in);
	}
	cout << "File was found at: " << fileLocation << '\n' << endl;
}

// display Finite State Machine
void Lexer::DisplayTable() {
	const int row_count = 7;
	const int col_count = 4;
	cout << "L = letters    # = pound sign    D = digits    . = period " << endl
		<< endl;
	for (int i = 0; i < row_count; i++) {
		if (i == 0) cout << "     \t\tL\t#\tD\t." << endl;
		cout << "row: " << i << "\t\t";
		for (int j = 0; j < col_count; j++) cout << Table[i][j] << "\t";
		cout << endl;
	}
	cout << endl;
}

// whenever a token is ready to be passed it's type is determined then outputted
void Lexer::Parse(string theToken) {
	lexBU = theToken;
	if (theToken == "\0")
		return;
	else if (isID == 1)
		tokenID = "IDENTIFIER";
	else if (isInt == 1)
		tokenID = "INTEGER";
	else if (isFloat == 1)
		tokenID = "FLOAT";
	else if (isKey == 1)
		tokenID = "KEYWORD";
	else if (isSeparator == 1) {
		if (tokenBU == NULL) {
			isSeparator = 0;
			lexeme = "";
			currentState = 0;
			return;
		}
		else {
			tokenID = "SEPARATOR";
			isSeparator = 0;
		}
	}
	else if (isOperator == 1) {
		tokenID = "OPERATOR";
		isOperator = 0;
	}
	// output token Type and Lexeme
	cout << left << setw(18) << tokenID << left << theToken << endl;

	// re-initialize all values
	isID = tokReady = isInt = isFloat = isKey = 0;
	isOperator = isSeparator = 0;
	lexeme = "", tokenID = "";
	currentState = 0;
}

// return error if anything goes wrong
void Lexer::ReturnError() {
	char getter;
	getter = file.get();
	while (true) {
		if (getter == ' ' || getter == '\n' || getter == '\t' || getter == '[' ||
			getter == ']' || getter == '(' || getter == ')' || getter == '{' ||
			getter == '}' || getter == (char)-1)
			break;
		cout << "getter: " << getter << " ascii: " << (int)getter << endl;
		lexeme += getter;
		getter = file.get();
	}

	cout << "\n\nToken not found: " << lexeme << endl;
	system("pause");
	exit(1);
}

// loops until a full token is gotten then return when ready
string Lexer::GetToken() {
	int inputType;
	string returnToken;

	// loop until end of file or when token ready
	while (!file.eof() && currentState != -1) {
		// get character and identify type //
		token = file.get();
		inputType = TableInput(token);

		// number followed by space or separator //
		if (currentState == 1 && inputType == 6) {
			returnToken = LookupState(currentState);
			file.unget();
			return returnToken;
		}
		// number followed by an operator //
		if (currentState == 1 && inputType == 7) {
			returnToken = LookupState(currentState);
			file.unget();
			return returnToken;
		}
		// float followed by space or separator //
		if (currentState == 6 && inputType == 6) {
			returnToken = LookupState(currentState);
			file.unget();
			return returnToken;
		}
		// float followed by an operator //
		if (currentState == 6 && inputType == 7) {
			returnToken = LookupState(currentState);
			file.unget();
			return returnToken;
		}
		// ID followed by a space or separator //
		if (currentState == 2 | currentState == 3 | currentState == 4 &&
			inputType == 6) {
			returnToken = LookupState(currentState);
			file.unget();
			return returnToken;
		}
		// ID followed by an operator //
		if (currentState == 2 | currentState == 3 | currentState == 4 &&
			inputType == 7) {
			returnToken = LookupState(currentState);
			file.unget();
			return returnToken;
		}
		// separator //
		else if (inputType == 6) {
			if (token == ' ' | token == '\t' | token == '\n') {
				return "";
			}
			else {
				returnToken = LookupSeparator();
				return returnToken;
			}

		}
		// operator //
		else if (inputType == 7) {
			returnToken = LookupOperator();
			return returnToken;
		}
		currentState = Table[currentState][inputType];

		// if not ready then keep adding tokens to lexeme string
		if (tokReady != 1) {
			lexeme += tokenBU = token;
		}
		else
			tokenBU = token;

		// invalid input
		if (currentState == -1) {
			ReturnError();
			returnToken = "";
			break;
		}
		// The token is ready to return
		if (tokReady == 1) break;
	}
	if (returnToken == "\n") {
		lineCounter++;
		charCounter = 1;
	}
	return returnToken;
}

// checks if in the final state and which type
string Lexer::LookupState(int n) {
	switch (n) {
	case (0):
		ReturnError();
		tokReady = 1;
		lexeme = "\0";
		currentState = -1;
		return lexeme;
	case (1):
		tokReady = 1;
		isInt = 1;
		currentState = 0;
		return lexeme;
	case (2):
		if (IsKeyword(lexeme)) return lexeme;
		tokReady = 1;
		isID = 1;
		currentState = 0;
		return lexeme;
	case (3):
		if (IsKeyword(lexeme)) return lexeme;
		tokReady = 1;
		isID = 1;
		currentState = 0;
		return lexeme;
	case (4):
		if (IsKeyword(lexeme)) return lexeme;
		tokReady = 1;
		isID = 1;
		currentState = 0;
		return lexeme;
	case (6):
		tokReady = 1;
		isFloat = 1;
		currentState = 0;
		return lexeme;
	}
	return "";
}

// check for special separators
string Lexer::LookupSeparator() {
	string returnTemp;
	returnTemp = token;
	char *space;
	if (token == ':') {
		char temp = file.get();
		if (temp == '=') {
			returnTemp += temp;
			token = NULL;
			currentState = 0;
			lexeme = "";
			return returnTemp;
		}
		else {
			file.unget();
			return returnTemp;
		}
	}
	if (token == '%') {
		char temp = file.get();
		if (temp == '%') {
			returnTemp += temp;
			isSeparator = 1;
			token = NULL;
			currentState = 0;
			lexeme = "";
			return returnTemp;
		}
		else {
			file.unget();
			return returnTemp;
		}
	}
	else {
		return returnTemp;
	}
}

// lookup special operators
string Lexer::LookupOperator() {
	string returnTemp = "";
	returnTemp += token;

	if (token == '!' || token == '=') {
		char temp = file.get();
		if (temp == '=') {
			returnTemp += temp;
			token = NULL;
			currentState = 0;
			lexeme = "";
			return returnTemp;
		}

		// cannot be ! by itself
		else {
			file.unget();
			if (token == '!') {
				ReturnError();
			}
			// operator is = sign
			if (token == '=') {
				returnTemp = token;
				token = NULL;
				currentState = 0;
				lexeme = "";
				return returnTemp;
			}
			else {
				// file.unget();
				returnTemp = token;
				token = NULL;
				currentState = 0;
				lexeme = "";
				return returnTemp;
			}
		}
	}
	// regular operator
	else {
		returnTemp = token;
		token = NULL;
		currentState = 0;
		lexeme = "";
		return returnTemp;
	}
}

// determines the input into the finite state machine
int Lexer::TableInput(char c) {
	if (c == ' ' | c == '\t' | c == '\n') return 6;
	if (isalpha(c))
		return 0;  // char is alphabetical
	else if (c == '#')
		return 1;  // char is pound sign
	else if (isdigit(c))
		return 2;  // char is a number
	else if (c == '.')
		return 3;  // char is period
	else {
		if (CheckSeparator(c))
			return 6;  // char is a separator
		else if (CheckOperator(c))
			return 7;  // char is an operator
		else
			return 6;
	}
}

// checks if char is separator --> then triggers lookupSeparator for special cases
bool Lexer::CheckSeparator(char c) {
	// check if char is a separator in the list
	char *temp;
	temp = find(Separators, Separators + 16, c);
	if (temp != Separators + 16) {
		isSeparator = 1;
		tokenBU = c;
		return 1;
	}
	// otherwise char is not a separator
	else {
		temp = NULL;
		return 0;
	}
}

// checks if char is operator --> then triggers lookupOperator for special cases
bool Lexer::CheckOperator(char c) {
	// check if char is a operator //
	char *temp;
	temp = find(Operators, Operators + 10, c);
	if (temp != Operators + 10) {
		isOperator = 1;
		return 1;
	}

	// check for ! in case of != operator
	else if (c == '!') {
		isOperator = 1;
		return 1;
	}
	else
		return 0;
	temp = NULL;
}

// check if the lexeme is in the list of defined keywords
bool Lexer::IsKeyword(string lex) {
	string *temp = find(Keywords, Keywords + 12, lex);
	if (temp != Keywords + 12) {
		lexeme = lex;
		tokReady = 1;
		currentState = 0;
		isKey = 1;
		return 1;
	}
	else
		return 0;
}

// check if the end of file or error occured
bool Lexer::IsEOF() {
	if (token == -1 || currentState == -1)
		return true;
	else
		return false;
}
