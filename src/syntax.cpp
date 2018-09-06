// Imple of Syntax class

#include <iostream>
#include "syntax.h"

bool Syntax::error = false;

Syntax::Syntax():indent(0){}

void Syntax::syntaxError(std::string msg){
	std::cout<<std::endl<<">>> Syntax error at line "<<Syntax::linenum<<": "<<msg;
	error = true;
}

void Syntax::match(TokenType expected){
	if(token == expected)
		token = lexs.getToken();
	else{
		syntaxError("unexpected token -> ");
		lexs.printToken(expected);
	}
}

SyntaxNode* Syntax::SyntaxAnalysis(){
	SyntaxNode* tree = NULL;
	token = lexs.getToken();
	tree = sequence_stmt();
	if(token != ENDFILE){
		syntaxError("Code ends before file");
	}
	
	return tree;
}

SyntaxNode* Syntax::sequence_stmt(){
	SyntaxNode* seq = stmt();
	SyntaxNode* p = seq;
	match(SEMI);
	while((token!=ENDFILE)&&(token!=ERROR)){
		SyntaxNode* q = stmt();
		if(q != NULL){
			if(seq == NULL)
				seq = p = q;
			else{
				p->sibling = q;
				p = q;
			}
		}
		match(SEMI);
	}
	
	return seq;
}
SyntaxNode* Syntax::stmt(){
	SyntaxNode* st = NULL;
	std::string tname;
	switch(token){
	case ID:
		tname = lexs.getMsg();
		match(ID);
		if(token == ASSIGN){
			st = assign_stmt(tname);
		}else{
			st = opera_stmt(tname);
		}
	break;
	case OUTPUT:
		st = output_stmt();
	break;
	default:
		syntaxError("unexpected token -> ");
		lexs.printToken(token);
		token = lexs.getToken();
	break;
	}
	
	return st;
}
SyntaxNode* Syntax::opera_stmt(std::string id){
	SyntaxNode* t = newStmtType(OperaType);
	if(t!=NULL){
		t->name = id;
		t->attr.opt = token;
		if(token>=PLUSASSG && token <=OVERMINUASSG){
			t->ok = true;
		}else{
			syntaxError("unexpected token -> ");
			lexs.printToken(token);
			token = lexs.getToken();
			return t;
		}
		match(token);
		t->child[0] = factor();
	}
	
	return t;
}
SyntaxNode* Syntax::assign_stmt(std::string id){
	SyntaxNode* t = newStmtType(AssignType);
	if(t!=NULL){
		t->name = id;
		t->attr.opt = token;
		t->ok = true;
		match(ASSIGN);
		t->child[0] = exp();
	}
	
	return t;
}
SyntaxNode* Syntax::output_stmt(){
	SyntaxNode* t = newStmtType(OutputType);
	match(OUTPUT);
	if(t!=NULL){
		t->ok = true;
		t->child[0] = factor();
	}
	
	return t;
}
SyntaxNode* Syntax::exp(){
	SyntaxNode* t = term();
	while(token==PLUS || token==MINUS){
		SyntaxNode* p = newExpType(OptType);
		if(p!=NULL){
			p->child[0] = t;
			p->attr.opt = token;
			p->ok = true;
			t = p;
			match(token);
			t->child[1] = term();
		}
	}
	
	return t;
}
SyntaxNode* Syntax::term(){
	SyntaxNode* t = factor();
	while ((token==TIMES)||(token==OVER)){
		SyntaxNode* p = newExpType(OptType);
		if(p!=NULL){
			p->child[0] = t;
			p->attr.opt = token;
			p->ok = true;
			t = p;
			match(token);
			t->child[1] = factor();
		}
	}
	
	return t;
}
SyntaxNode* Syntax::factor(){
	SyntaxNode* t = NULL;
	switch(token){
	case ID:
		t = newExpType(IdType);
		if(t!=NULL){
			t->name = lexs.getMsg();
			t->ok = true;
		}
		match(ID);
	break;
	case NUM:
		t = newExpType(NumType);
		if(t!=NULL){
			t->attr.val = lexs.getMsgInt();
			t->type = INTER;
			t->ok = true;
		}
		match(NUM);
	break;
	case LPAREN:
		t = newExpType(FacType);
		match(LPAREN);
		if(t!=NULL){
			t->attr.opt = LPAREN;
			t->child[0] = exp();
			t->ok = true;
		}
		match(RPAREN);
	break;
	default:
		syntaxError("unexpected token -> ");
		lexs.printToken(token);
		token = lexs.getToken();
	break;
	}
	
	return t;
}

SyntaxNode* Syntax::newStmtType(StmtT kind){
	SyntaxNode* node = new SyntaxNode();
	if(node == NULL)
		std::cout<<"Out of memory error at line "<<Syntax::linenum<<std::endl;
	else{
		for(int i=0; i<MAXCHILDLEN; i++)
		node->child[i] = NULL;
		node->sibling = NULL;
		node->nodeType = StmtType;
		node->nodeKind.stmt = kind;
		node->linenum = Syntax::linenum;
		node->ok = false;
	}
	
	return node;
}

SyntaxNode* Syntax::newExpType(ExpT kind){
	SyntaxNode* node = new SyntaxNode();
	if(node == NULL)
		std::cout<<"Out of memory error at line "<<Syntax::linenum<<std::endl;
	else{
		for(int i=0; i<MAXCHILDLEN; i++)
		node->child[i] = NULL;
		node->sibling = NULL;
		node->nodeType = ExpType;
		node->nodeKind.exp = kind;
		node->linenum = Syntax::linenum;
		node->ok = false;
	}
	
	return node;
}

void Syntax::DeleteSyntaxTree(SyntaxNode* root){
	if(root!=NULL){
		for(int i=0; i<MAXCHILDLEN; i++){
			if(root->child[i] != NULL){
				DeleteSyntaxTree(root->child[i]);
				root->child[i] = NULL;
			}
		}
		if(root->sibling !=NULL){
			DeleteSyntaxTree(root->sibling);
			root->sibling = NULL;
		}
		
		delete root;
		root = NULL;
	}
}

void Syntax::printSyntaxTree(SyntaxNode* root){
	using std::cout;
	indent += 2;
	while(root!=NULL){
		printSpaces();
		if(root->nodeType == StmtType){
			switch(root->nodeKind.stmt){
			case OperaType:
				if(root->ok){
					cout<<"Operator Assign>> Left ID: "<<root->name<<"\t\tOption: ";
					lexs.printToken(root->attr.opt);
					printSpaces();
					cout<<"               >> Right Factor: "<<std::endl;
				}else{
					cout<<"Operator Assign>> Error Node."<<std::endl;
				}
			break;
			case AssignType:
				if(root->ok){
					cout<<"Assign>> Variable: "<<root->name<<"\t\tOption: ";
					lexs.printToken(root->attr.opt);
					printSpaces();
					cout<<"      >> Right Expression:"<<std::endl;
				}else{
					cout<<"Assign>> Error Node."<<std::endl;
				}
			break;
			case OutputType:
				if(root->ok){
					cout<<"Output>> Factor:"<<std::endl;
				}else{
					cout<<"Output>> Error Node."<<std::endl;
				}
			break;
			}
		}else if(root->nodeType == ExpType){
			switch(root->nodeKind.exp){
			case OptType:
				if(root->ok){
					cout<<"option: ";
					lexs.printToken(root->attr.opt);
				}else{
					cout<<"option: error op."<<std::endl;
				}
			break;
			case NumType:
				if(root->ok){
					cout<<"number: "<<root->attr.val<<std::endl;
				}else{
					cout<<"number: error num."<<std::endl;
				}
			break;
			case IdType:
				if(root->ok){
					cout<<"id: "<<root->name<<std::endl;
				}else{
					cout<<"id: error id."<<std::endl;
				}
			break;
			case FacType:
				if(root->ok){
					cout<<"(factor exp): "<<std::endl;
				}else{
					cout<<"factor error."<<std::endl;
				}
			break;
			}
		}else{
			cout<<"Unknown Node Type"<<std::endl;
		}
		for(int i=0; i<MAXCHILDLEN; i++)
			printSyntaxTree(root->child[i]);
		root = root->sibling;
	}
	indent -= 2;
}

void Syntax::printSpaces(){
	std::string space(indent*2, ' ');
	std::cout<<space;
}
