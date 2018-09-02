// Imple of codemk class
#include "codemk.h"
#include <iomanip>
using std::string;

bool Codemk::error = false;
string* Codemk::codename = NULL;
int Codemk::indxno = 0;

void Codemk::setFilename(string* name){
	codename = name;
}

void Codemk::getStmt(SyntaxNode* tree){
	
}

void Codemk::getExp(SyntaxNode* tree){
	
}

void Codemk::cGEN(SyntaxNode* tree){
	if (tree != NULL){
		switch(tree->nodeType){
		case StmtType:
			getStmt(tree);
		break;
		case ExpType:
			getExp(tree);
		break;
		default:
			error = true;
		break;
		}
		cGEN(tree->sibling);
	}
}

void Codemk::CodeGEN(SyntaxNode* tree){
	emitInfo(".file\t"+(*codename));
	emitInfo(".comment");
	emitComment("Simple Compilation Compiler Code");
	emitComment("File standard infomation comment");
	emitComment("END of comment prelude.");
	emitInfo(".code");
	indxno++;
	emitRM("MOV", 0, ENM, 0, EAX);
	emitRM("LOD", 0, EIP, 0, ENLL);
	emitRM("MOV", 0, ENM, 0, EBP);
	cGEN(tree);
	emitRM("HALT", 0, ENM, 0, ENM);
	emitComment("END of File comment");
}

void Codemk::emitInfo(string msg){
	(*codefile) << "\t"+msg << std::endl;
}

void Codemk::emitComment(string msg){
	if (flag&0x00001000){
		(*codefile) << "**\t"+msg <<std::endl;
	}
}

void Codemk::emitRM(string opt, int num1, RegType type1, int num2, RegType type2){
	(*codefile) << std::setw(3)<< std::setfill('0')<< indxno++;
	(*codefile) << "\t"<< opt;
	if(type1 != ENLL){
		(*codefile) << "\t";
		if(type1 == ENM)	(*codefile) << "$"<< num1;
		else if(num1 == 0)	(*codefile) << "%"<< RegTab[(int)type1];
		else				(*codefile) << num1<< "(%"<< RegTab[(int)type1]<<")";
	}
	
	if (type2 != ENLL){
		(*codefile) << ",\t";
		if(type2 == ENM)	(*codefile) << "$"<< num2;
		else if(num2 == 0)	(*codefile) << "%"<< RegTab[(int)type2];
		else				(*codefile) << num2<< "(%"<< RegTab[(int)type2]<<")";
	}
	(*codefile) << std::endl;
}

void Codemk::emitRO(){

}

void Codemk::emitSkip(){

}
