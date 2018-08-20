// code maker class

#ifndef __CODEMK_H__
#define __CODEMK_H__

#include "basic.h"

class Codemk : public Basic{
public:
	static bool error;
	static void setFilename(std::string* name);
	void CodeGEN(SyntaxNode* tree);
protected:
	static std::string* codename;
private:
	void emitInfo(std::string info);
	void emitComment(std::string msg);
	void emitRM();
	void emitRO();
	void emitSkip();
};


#endif