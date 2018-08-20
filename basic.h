// basic class

// stroe global variable and func

#ifndef __BASIC_H__
#define __BASIC_H__

#include <fstream>
#include <string>

#ifndef EOF
#define EOF (-1)
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#define MAXCHILDLEN 3

typedef enum {
	ENDFILE, ERROR, 
	ID, NUM, 
	OUTPUT, SEMI, LPAREN, RPAREN, ASSIGN, PLUS, MINUS, TIMES, OVER,
	PLUSASSG, MINUASSG, TIMEASSG, OVERASSG, PLUSTIMEASSG, PLUSOVERASSG,
	MINUTIMEASSG, MINUOVERASSG, TIMEPLUSASSG, TIMEMINUASSG, OVERPLUSASSG, OVERMINUASSG
} TokenType;

typedef enum { StmtType, ExpType } NodeType;
typedef enum { OperaType, AssignType, OutputType } StmtT;
typedef enum { OptType, NumType, IdType, FacType } ExpT;
typedef enum { VOID, INTER } CheckType;

class SyntaxNode{
public:
	bool ok;
	SyntaxNode* child[MAXCHILDLEN];
	SyntaxNode* sibling;
	int linenum;
	NodeType nodeType;
	CheckType type;
	union { StmtT stmt; ExpT exp; } nodeKind;
	union { TokenType opt; int val; } attr;
	std::string name;
};

class Basic{
public:
	static void setSource(std::ifstream* fp);
	static void setFlag(int f);
	static void setCode(std::ofstream* fp);
	static std::ifstream* getSource();
protected:
	static int flag;
	static int linenum;
	static std::ifstream* source;
	static std::ofstream* codefile;
};

#endif  // basic.h
