#pragma once
// Lexer.h
#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <iomanip>
using namespace std;

class Lexer {
private:
	int lineCounter = 1;
	int charCounter = 1;
	vector<vector<int>> Table;
	ifstream file;
	string fileLocation;
	int currentState = 0;
	char tokenBU;
	string lexeme;
	char token;
	string tokenID;
	string lexBU;
	bool isSeparator, isOperator, isID, isInt, isFloat, isKey, tokReady, isSpace;

public:
	// class constructor
	Lexer() {
		ConstructTable();
		GetTextFile(file);
	}
	~Lexer() { file.close(); }
	// function Prototypes
	void ConstructTable();
	void GetTextFile(ifstream &file);
	void Parse(string theToken);
	void ReturnError();
	void DisplayTable();
	string GetToken();
	string LookupState(int n);
	string LookupSeparator();
	string LookupOperator();
	int TableInput(char c);
	bool CheckSeparator(char c);
	bool CheckOperator(char c);
	bool IsKeyword(string lex);
	bool IsEOF();
};

#endif