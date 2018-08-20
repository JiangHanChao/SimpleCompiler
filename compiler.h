// the Compiler class

#ifndef __SIMPLECOMPILER_H__
#define __SIMPLECOMPILER_H__

#include "basic.h"
#include "codemk.h"
#include "syntax.h"
#include "analysis.h"

class SimpleCompiler {
public:
	void run();
	void setFlag(int flag);
	void setCode(std::ofstream* code);
	void setSource(std::ifstream* fp);
	void setCodename(std::string name);
private:
	int compflag;
	Codemk codemk;
	Syntax syntax;
	Analysis analyze;
	std::string codename;
};

#endif

