// Imple of codemk class
#include "codemk.h"
using std::string;

bool Codemk::error = false;
string* Codemk::codename = NULL;

void Codemk::setFilename(string* name){
	codename = name;
}

void Codemk::CodeGEN(SyntaxNode* tree){
	emitInfo(".file\t"+(*codename));
	emitInfo(".comment");
	emitComment("Simple Compilation Compiler Code");
	emitComment("File standard infomation comment");
	
	emitComment("Standard Infomation comment End");
	emitInfo(".code");
}

void Codemk::emitInfo(string msg){
	(*codefile) << "\t"+msg << std::endl;
}

void Codemk::emitComment(string msg){
	if (flag&0x00001000){
		(*codefile) << "**\t"+msg <<std::endl;
	}
}