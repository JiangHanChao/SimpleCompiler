// implemenetation of Compiler class
#include "compiler.h"

#include <iostream>

void SimpleCompiler::setSource(std::ifstream* fp){
	Syntax::setSource(fp);
}

void SimpleCompiler::setFlag(int flag){
	compflag = flag;
	Syntax::setFlag(flag);
}

void SimpleCompiler::setCodename(std::string name){
	codename = name;
	Codemk::setFilename(&codename);
}

void SimpleCompiler::run(){
	std::ofstream code;
	SyntaxNode* tree = syntax.SyntaxAnalysis();
	
	if(compflag&0x00000400){
		std::cout<<"Syntax Analysis Tree:"<<std::endl;
		syntax.printSyntaxTree(tree);
	}
	
	if(Syntax::error){
		std::cout<<"Compiler file failed."<<std::endl;
		goto ptree;
	}

	analyze.BuildSymTable(tree);
	if (analyze.checkError()){
		std::cout<<std::endl<<"Building Symbol Table Failed."<<std::endl;
		goto ptree;
	}
	std::cout<<"\nBuilding Symbol Table Finished."<<std::endl;
	
	analyze.typeCheck(tree);
	if (analyze.checkError()){
		std::cout<<std::endl<<"Analysis Syntax tree Type failed."<<std::endl;
		goto ptree;
	}
	std::cout<<"Analysis Syntax tree Type Finished."<<std::endl;
	
	code.open(codename.c_str(), std::ios::out | std::ios::trunc);
	if (code.fail()){
		std::cout<<"code output file <"<<codename<<"> can\'t created or open failed."<<std::endl;
		goto ptree;
	}
	Syntax::setCode(&code);

	std::cout<<"Simple Compiler to make run code files <"<<codename<<">..."<<std::endl;
	codemk.CodeGEN(tree);
	if (Codemk::error){
		std::cout<<std::endl<<"code making has errors and failed."<<std::endl;
		std::cout<<"Simple Compiler Compilation Failure."<<std::endl;
	}else{
		std::cout<<"code file making Finished."<<std::endl;
		std::cout<<"Simple Compiler Compilation Successful."<<std::endl;
	}
	code.close();
	
ptree:
	syntax.DeleteSyntaxTree(tree);
}
