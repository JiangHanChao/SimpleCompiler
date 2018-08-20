// lexical analysis

#ifndef __LEXAY_H__
#define __LEXAY_H__

#include "basic.h"

#define LINESTRINGLEN 128

class Lexay : public Basic{
private:
	static int linepos;
	static int linelen;
	static bool lineflag;
	static std::string currMsg; 
	static std::string currline;
	enum StateType {
		START, INNUM, INID, INPLUS, INMINUS, INTIMES, INOVER, INPLUSTIMES, INPLUSOVER,
		INMINUTIMES, INMINUOVER, INTIMESPLUS, INTIMESMINU, INOVERPLUS, INOVERMINU, END
	};
	int getChar();
	void RollbackChar();
	TokenType RelookReservedType();
public:
	TokenType getToken();
	void printToken(TokenType type);
	std::string getMsg();
	int getMsgInt();
	void testLex();
};

#endif

