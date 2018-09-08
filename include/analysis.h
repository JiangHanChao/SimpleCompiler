// analysis class

#ifndef __ANALYSIS_H__
#define __ANALYSIS_H__

#include "basic.h"

class LineList
{
public:
	int linenum;
	LineList* next;
};

class BucketList
{
public:
	CheckType type;
	size_t memloc;
	LineList* lines;
	std::string name;
	BucketList* next;
};

class Symtab {
public:
	~Symtab();
	void insert(std::string name, int lineno, int loc);
	static size_t lookup(std::string name);
	CheckType lookType(std::string name);
	void setType(std::string name, CheckType type);
	void printTable();
	static int getTableSize();
protected:
	static int hash(std::string key);
private:
	static const int hash_size = 211;
	static const int hash_shift = 4;
	static BucketList* HASHTABLE[hash_size];
};

class Analysis : public Basic{
public:
	void BuildSymTable(SyntaxNode* tree);
	void typeCheck(SyntaxNode* tree);
	bool checkError();
protected:
	void tracelook(SyntaxNode* tree, bool check=false);
	void InsertNode(SyntaxNode* t);
	void typeError(SyntaxNode* t, std::string errmsg);
	void checkNode(SyntaxNode* t);
private:
	static bool error;
	static int location;
	Symtab st;
};

#endif

