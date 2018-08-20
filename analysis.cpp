// Imple of Analysis class

#include <iostream>
#include "analysis.h"
using std::string;

BucketList* Symtab::HASHTABLE[hash_size] = {NULL};

int Analysis::location = 0;
bool Analysis::error = false;

Symtab::~Symtab(){
	for (int i = 0; i < hash_size; ++i){
		if (HASHTABLE[i] != NULL){
			BucketList* bl = HASHTABLE[i];
			HASHTABLE[i] = NULL;
			while(bl != NULL){
				LineList* ll = bl->lines;
				bl->lines = NULL;
				while(ll != NULL){
					LineList* freell = ll;
					ll = ll->next;
					freell->next = NULL;
					delete freell;
				}
				BucketList* freebl = bl;
				bl = bl->next;
				freebl->next = NULL;
				delete freebl;
			}
		}
	}
}

int Symtab::hash(string key){
	int temp = 0;
	int len = key.length();
	for (int i = 0; i < len; ++i){
		temp = ((temp << hash_shift) + key[i]) % hash_size;
	}
	return temp;
}

void Symtab::insert(string name, int lineno, int loc){
	int h = hash(name);
	BucketList* bl = HASHTABLE[h];
	while(bl != NULL && name.compare(bl->name) != 0)
		bl = bl->next;
	if (bl == NULL){
		bl = new BucketList();
		bl->type = VOID;
		bl->memloc = loc;
		bl->lines = new LineList();
		bl->lines->linenum = lineno;
		bl->lines->next = NULL;
		bl->name = name;
		bl->next = HASHTABLE[h];
		HASHTABLE[h] = bl;
	}else{
		LineList* ll = bl->lines;
		while(ll->next != NULL)	ll = ll->next;
		ll->next = new LineList();
		ll->next->linenum = lineno;
		ll->next->next = NULL;
	}
	return ;
}

size_t Symtab::lookup(string name){
	int h = hash(name);
	BucketList* bl = HASHTABLE[h];
	while(bl != NULL && name.compare(bl->name) != 0)
		bl = bl->next;
	if (bl == NULL)
		return -1;
	else
		return bl->memloc;
}

CheckType Symtab::lookType(string name){
	int h = hash(name);
	BucketList* bl = HASHTABLE[h];
	while(bl != NULL && name.compare(bl->name) != 0)
		bl = bl->next;
	if (bl == NULL){
		return VOID;
	}else
		return bl->type;
}

void Symtab::setType(string name, CheckType type){
	int h = hash(name);
	BucketList* bl = HASHTABLE[h];
	while(bl != NULL && name.compare(bl->name) != 0)
		bl = bl->next;
	if (bl != NULL){
		bl->type = type;
	}
}

void Symtab::printTable(){
	using std::cout; using std::ios;
	cout<<"Variable Name  Location   Line Numbers"<<std::endl
		<<"-------------  --------   ------------"<<std::endl;
	cout.setf(ios::left);

	for (int i = 0; i < hash_size; ++i){
		if (HASHTABLE[i] != NULL){
			BucketList* bl = HASHTABLE[i];
			while(bl != NULL){
				LineList* ll = bl->lines;
				cout.width(15);
				cout<<bl->name;
				cout.width(11);
				cout<<bl->memloc;
				while(ll != NULL){
					cout.width(4);
					cout<<ll->linenum;
					ll = ll->next;
				}
				cout<<std::endl;
				bl = bl->next;
			}
		}
	}

	cout.unsetf(ios::left);
}

void Analysis::BuildSymTable(SyntaxNode* tree){
	tracelook(tree);
	if(flag&0x00000800){
		std::cout<<std::endl<<"Symbol Table:"<<std::endl;
		st.printTable();
	}
	return ;
}

void Analysis::tracelook(SyntaxNode* tree, bool check){
	if(tree!=NULL){
		if (!check)	InsertNode(tree);
		for(int i=0; i<MAXCHILDLEN; i++)
			tracelook(tree->child[i], check);
		if (check)	checkNode(tree);
		tracelook(tree->sibling, check);
	}
	return ;
}

void Analysis::InsertNode(SyntaxNode* t){
	switch(t->nodeType){
		case StmtType:
			switch(t->nodeKind.stmt){
			case OperaType:
			case AssignType:
				if (!t->name.empty() && t->ok){
					if (st.lookup(t->name) == -1){
						st.insert(t->name, t->linenum, location++);
					}else{
						st.insert(t->name, t->linenum, 0);
					}
				}
			break;
			default:
			break;
			}
		break;
		case ExpType:
			switch(t->nodeKind.exp){
			case IdType:
				if (!t->name.empty() && t->ok){
					if (st.lookup(t->name) == -1){
						st.insert(t->name, t->linenum, location++);
					}else{
						st.insert(t->name, t->linenum, 0);
					}
				}
			break;
			default:
			break;
			}
		break;
		default:
		break;
	}
	return ;
}

void Analysis::typeError(SyntaxNode* t, string errmsg){
	std::cout<<"Type Error at line "<<t->linenum<<": "<<errmsg<<std::endl;
	Analysis::error = true;
}

void Analysis::typeCheck(SyntaxNode* tree){
	tracelook(tree, true);
}

void Analysis::checkNode(SyntaxNode* t){
	if (!t->ok)	typeError(t, "SyntaxNode ok_flag is false.");
	switch(t->nodeType){
		case StmtType:
			switch(t->nodeKind.stmt){
			case OperaType:
				if (t->child[0]->type != INTER){
					t->ok = false;
					typeError(t->child[0], "operator arguments not a interger type.");
				}
				st.setType(t->name, INTER);
			break;
			case AssignType:
				if (t->child[0]->type != INTER){
					t->ok = false;
					typeError(t->child[0], "assign right not a interger type.");
				}
				st.setType(t->name, INTER);
			break;
			case OutputType:
				if (t->child[0]->type != INTER){
					t->ok = false;
					typeError(t->child[0], "output not a interger type.");
				}
				t->type = VOID;
			break;
			default:
			break;
			}
		break;
		case ExpType:
			switch(t->nodeKind.exp){
			case IdType:
				t->type = st.lookType(t->name);
				if(t->type != INTER){
					t->ok = false;
					typeError(t, t->name+" not initializated.");
				}
			break;
			case OptType:
				if (t->child[0]->type==INTER && t->child[1]->type==INTER){
					t->type = INTER;
				}else{
					t->ok = false;
					typeError(t, "option arguments not a interger type.");
				}
			break;
			case NumType:
				if (t->type != INTER){
					t->ok = false;
					typeError(t, "Numbers get error type.");
				}
			break;
			case FacType:
				if (t->child[0]->type==INTER){
					t->type = INTER;
				}else{
					t->ok = false;
					typeError(t->child[0],"factors not a interger type.");
				}
			break;
			default:
			break;
			}
		break;
		default:
		break;
	}
	return ;
}

bool Analysis::checkError(){
	return error;
}
