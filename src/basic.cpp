// Imple of Basic
#include "basic.h"

int Basic::flag = (2<<8);
int Basic::linenum = 0;
std::ifstream* Basic::source = NULL;
std::ofstream* Basic::codefile = NULL;

void Basic::setSource(std::ifstream* fp){
	source = fp;
}

void Basic::setFlag(int f){
	flag = f;
}

void Basic::setCode(std::ofstream* fp){
	codefile = fp;
}

std::ifstream* Basic::getSource(){
	return source;
}