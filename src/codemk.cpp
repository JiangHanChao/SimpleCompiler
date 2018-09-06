// Imple of codemk class
#include "codemk.h"
#include "analysis.h"
#include <iomanip>
using std::string;

bool Codemk::error = false;
string* Codemk::codename = NULL;
int Codemk::indxno = 0;

void Codemk::setFilename(string* name){
	codename = name;
}

void Codemk::getStmt(SyntaxNode* tree){
	int loc;
	switch(tree->nodeKind.stmt){
	case OperaType:
		emitComment("-> operator");
		cGEN(tree->child[0]);
		loc = Symtab::lookup(tree->name);
		emitRM("MOV", loc, EBP, 0, EBX);
		switch(tree->attr.opt){
		case PLUSASSG:
			emitRM("ADD", 0, EAX, 0, EBX);
			break;
		case MINUASSG:
			emitRM("SUB", 0, EAX, 0, EBX);
			break;
		case TIMEASSG:
			emitRM("MUL", 0, EAX, 0, EBX);
			break;
		case OVERASSG:
			emitRM("DIV", 0, EAX, 0, EBX);
			break;
		case PLUSTIMEASSG:
			emitRM("MUL", 0, EBX, 0, EAX);
			emitRM("ADD", 0, EAX, 0, EBX);
			break;
		case PLUSOVERASSG:
			emitRM("MOV", 0, EBX, 0, ECX);
			emitRM("DIV", 0, EAX, 0, ECX);
			emitRM("ADD", 0, ECX, 0, EBX);
			break;
		case MINUTIMEASSG:
			emitRM("MUL", 0, EBX, 0, EAX);
			emitRM("SUB", 0, EAX, 0, EBX);
			break;
		case MINUOVERASSG:
			emitRM("MOV", 0, EBX, 0, ECX);
			emitRM("DIV", 0, EAX, 0, ECX);
			emitRM("SUB", 0, ECX, 0, EBX);
			break;
		case TIMEPLUSASSG:
			emitRM("ADD", 0, EBX, 0, EAX);
			emitRM("MUL", 0, EAX, 0, EBX);
			break;
		case TIMEMINUASSG:
			emitRM("MOV", 0, EBX, 0, ECX);
			emitRM("SUB", 0, EAX, 0, ECX);
			emitRM("MUL", 0, ECX, 0, EBX);
			break;
		case OVERPLUSASSG:
			emitRM("ADD", 0, EBX, 0, EAX);
			emitRM("DIV", 0, EAX, 0, EBX);
			break;
		case OVERMINUASSG:
			emitRM("MOV", 0, EBX, 0, ECX);
			emitRM("SUB", 0, EAX, 0, ECX);
			emitRM("DIV", 0, ECX, 0, EBX);
			break;
		default:
			emitComment("error");
			error = true;
			return ;
		}
		emitRM("MOV", 0, EBX, loc, EBP);
		emitComment("<- operator");
	break;
	case AssignType:
		emitComment("-> assign");
		cGEN(tree->child[0]);
		loc = Symtab::lookup(tree->name);
		emitRM("MOV", 0, EAX, loc, EBP);
		emitComment("store assign result to "+tree->name);
		emitComment("<- assign");
	break;
	case OutputType:
		emitComment("-> output");
		cGEN(tree->child[0]);
		emitRM("MOV",0, EAX, 0, ESP);
		emitCall("OUT");
		emitComment("<- output");
	break;
	default:
		error = true;
	break;
	}
	emitRM("MOV", 0, ENM, 0, EAX);
}

void Codemk::getExp(SyntaxNode* tree){
	int loc;
	switch(tree->nodeKind.exp){
	case OptType:
		switch(tree->attr.opt){
		case PLUS:
			cGEN(tree->child[1]);
			emitRM("MOV", 0, EAX, 0, EBX);
			cGEN(tree->child[0]);
			emitRM("ADD", 0, EBX, 0, EAX);
			break;
		case MINUS:
			cGEN(tree->child[1]);
			emitRM("MOV", 0, EAX, 0, EBX);
			cGEN(tree->child[0]);
			emitRM("SUB", 0, EBX, 0, EAX);
			break;
		case TIMES:
			cGEN(tree->child[0]);
			emitRM("MOV", 0, EAX, 0, ECX);
			cGEN(tree->child[1]);
			emitRM("MUL", 0, ECX, 0, EAX);
			break;
		case OVER:
			cGEN(tree->child[0]);
			emitRM("MOV", 0, EAX, 0, ECX);
			cGEN(tree->child[1]);
			emitRM("DIV", 0, ECX, 0, EAX);
			break;
		default:
			error = true;
			return;
		}
	break;
	case NumType:
		emitRM("MOV", tree->attr.val, ENM, 0, EAX);
	break;
	case IdType:
		loc = Symtab::lookup(tree->name);
		emitRM("MOV", loc, EBP, 0, EAX);
	break;
	case FacType:
		cGEN(tree->child[0]);
	break;
	default:
		error = true;
	break;
	}
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
	if(type1 !=ENLL){
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

void Codemk::emitCall(string interrupt){
	(*codefile) << std::setw(3)<< std::setfill('0')<< indxno++;
	(*codefile) << "\tCALL\t"<< interrupt<< std::endl;
}


