// Syntax Analysis tree Class

#ifndef __SYNTAX_H__
#define __SYNTAX_H__

#include "lexay.h"

class Syntax : public Basic{
private:
	int indent;
	Lexay lexs;
	TokenType token;
	void printSpaces();
	void match(TokenType expected);
	SyntaxNode* sequence_stmt();
	SyntaxNode* stmt();
	SyntaxNode* opera_stmt(std::string id);
	SyntaxNode* assign_stmt(std::string id);
	SyntaxNode* output_stmt();
	SyntaxNode* exp();
	SyntaxNode* term();
	SyntaxNode* factor();
	SyntaxNode* newStmtType(StmtT kind);
	SyntaxNode* newExpType(ExpT kind);
public:
	Syntax();
	static bool error;
	void printSyntaxTree(SyntaxNode* root);
	SyntaxNode* SyntaxAnalysis();
	void DeleteSyntaxTree(SyntaxNode* root);
	void syntaxError(std::string msg);
};

#endif

