// code maker class

#ifndef __CODEMK_H__
#define __CODEMK_H__

#include "basic.h"

enum RegType {
	EAX=0, EBX, ECX, EDX, EBP, ESP, EIP, ENM, ENLL=8
};
const char* const RegTab[] = {
	"eax", "ebx", "ecx", "edx", "ebp", "esp", "eip"
};

class Codemk : public Basic{
public:
	static bool error;
	static void setFilename(std::string* name);
	void CodeGEN(SyntaxNode* tree);
protected:
	static std::string* codename;
	static int  indxno;
private:
	void cGEN(SyntaxNode* tree);
	void getStmt(SyntaxNode* tree);
	void getExp(SyntaxNode* tree);
	void emitInfo(std::string info);
	void emitComment(std::string msg);
	void emitRM(std::string opt,int num,RegType type,int num2,RegType type2);
	void emitRO();
	void emitSkip();
};


#endif
