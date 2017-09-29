// Douglas Galm
// David Poss
// Due: 10 / 1 / 17
// Description: This program parses source code from a file and outputs token
// type and lexeme

#include <iostream>
#include "Lexer.h"

using namespace std;

int main() {
	cout << "Program Start" << endl;
	Lexer RAT17F;
	RAT17F.DisplayTable();
	cout << endl;
	while (!RAT17F.IsEOF()) {
		RAT17F.Parse(RAT17F.GetToken());
	}
	system("pause");
	return 0;
}
