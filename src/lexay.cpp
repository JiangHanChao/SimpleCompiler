// Imple of lexay class

#include "lexay.h"
#include <cctype>
#include <cstdlib>
#include <iostream>

int Lexay::linepos = 0;
int Lexay::linelen = 0;
bool Lexay::lineflag = false;
std::string Lexay::currline; 
std::string Lexay::currMsg;

static char linestring[LINESTRINGLEN];

int Lexay::getChar(){
	while(linepos >= linelen){
		linenum++;
		if(std::getline((*source), currline, '\n')){
			linelen = currline.size();
			linepos = 0;
			size_t pos = currline.find("//");
			if(pos != std::string::npos){
				currline = currline.substr(0, pos);
				linelen = currline.size();
			}
		}else{
			lineflag = true;
			return EOF;
		}
	}

	return currline[linepos++];
}

void Lexay::RollbackChar(){
	if(!lineflag){
		linepos--;
	}
}

TokenType Lexay::RelookReservedType(){
	if(!currMsg.compare("out")){
		return OUTPUT;
	}else
		return ID;
}

TokenType Lexay::getToken(){
	int tokenIndex = 0;
	TokenType currType;
	StateType state = START;
	bool save;
	
	while(state != END){
		int c = getChar();
		save = true;
		switch(state){
		case START:
			if(isdigit(c)){
				state = INNUM;
			}else if(isalpha(c)){
				state = INID;
			}else if((c == ' ') || (c == '\t')){
				save = false;
			}else if(c == '+'){
				state = INPLUS;
			}else if(c == '-'){
				state = INMINUS;
			}else if(c == '*'){
				state = INTIMES;
			}else if(c == '/'){
				state = INOVER;
			}else{
				state = END;
				if(c == EOF){
					save = false;
					currType = ENDFILE;
				}else if(c == '='){
					currType = ASSIGN;
				}else if(c == ';'){
					currType = SEMI;
				}else if(c == '('){
					currType = LPAREN;
				}else if(c == ')'){
					currType = RPAREN;
				}else{
					currType = ERROR;
				}
			}
		break;
		case INNUM:
			if(!isdigit(c)){
				RollbackChar();
				save = false;
				state = END;
				currType = NUM;
			}
		break;
		case INID:
			if((!isalpha(c)) && (!isdigit(c))){
				RollbackChar();
				save = false;
				state = END;
				currType = ID;
			}
		break;
		case INPLUS:
			if(c == '='){
				state = END;
				currType = PLUSASSG;
			}else if(c == '*'){
				state = INPLUSTIMES;
			}else if(c == '/'){
				state = INPLUSOVER;
			}else{
				RollbackChar();
				save = false;
				state = END;
				currType = PLUS;
			}
		break;
		case INMINUS:
			if(c == '='){
				state = END;
				currType = MINUASSG;
			}else if(c == '*'){
				state = INMINUTIMES;
			}else if(c == '/'){
				state = INMINUOVER;
			}else{
				RollbackChar();
				save = false;
				state = END;
				currType = MINUS;
			}
		break;
		case INTIMES:
			if(c == '='){
				state = END;
				currType = TIMEASSG;
			}else if(c == '+'){
				state = INTIMESPLUS;
			}else if(c == '-'){
				state = INTIMESMINU;
			}else{
				RollbackChar();
				save = false;
				state = END;
				currType = TIMES;
			}
		break;
		case INOVER:
			if(c == '='){
				state = END;
				currType = OVERASSG;
			}else if(c == '+'){
				state = INOVERPLUS;
			}else if(c == '-'){
				state = INOVERMINU;
			}else{
				RollbackChar();
				save = false;
				state = END;
				currType = OVER;
			}
		break;
		case INPLUSTIMES:
			state = END;
			if(c == '='){
				currType = PLUSTIMEASSG;
			}else{
				currType = ERROR;
			}
		break;
		case INPLUSOVER:
			state = END;
			if(c == '='){
				currType = PLUSOVERASSG;
			}else{
				currType = ERROR;
			}
		break;
		case INMINUTIMES:
			state = END;
			if(c == '='){
				currType = MINUTIMEASSG;
			}else{
				currType = ERROR;
			}
		break;
		case INMINUOVER:
			state = END;
			if(c == '='){
				currType = MINUOVERASSG;
			}else{
				currType = ERROR;
			}
		break;
		case INTIMESPLUS:
			state = END;
			if(c == '='){
				currType = TIMEPLUSASSG;
			}else{
				currType = ERROR;
			}
		break;
		case INTIMESMINU:
			state = END;
			if(c == '='){
				currType = TIMEMINUASSG;
			}else{
				currType = ERROR;
			}
		break;
		case INOVERPLUS:
			state = END;
			if(c == '='){
				currType = OVERPLUSASSG;
			}else{
				currType = ERROR;
			}
		break;
		case INOVERMINU:
			state = END;
			if(c == '='){
				currType = OVERMINUASSG;
			}else{
				currType = ERROR;
			}
		break;
		case END:
		default:
			std::cout<<"Lexay scanner BUG: state = "<<state<<std::endl;
			state = END;
			currType = ERROR;
		break;
		}
		if(save && (tokenIndex < LINESTRINGLEN))
			linestring[tokenIndex++] = (char)c;
		if(state == END){
			linestring[tokenIndex] = '\0';
			currMsg.assign(linestring);
			if(currType == ID)	currType = RelookReservedType();
		}
	}
	if(flag&0x00000100){
		std::cout<<linenum<<": ";
		printToken(currType);
	}
	return currType;
}

void Lexay::printToken(TokenType type){
	using std::cout;
	switch(type){
	case ID:  cout<<"ID name="<<currMsg<<std::endl; break;
	case NUM: cout<<"NUM val="<<currMsg<<std::endl; break;
	case OUTPUT: cout<<"output"<<std::endl; break;
	case ASSIGN: cout<<"="<<std::endl; break;
	case PLUS:	 cout<<"+"<<std::endl; break;
	case MINUS:  cout<<"-"<<std::endl; break;
	case TIMES:  cout<<"*"<<std::endl; break;
	case OVER:	 cout<<"/"<<std::endl; break;
	case SEMI:	 cout<<";"<<std::endl; break;
	case LPAREN: cout<<"("<<std::endl; break;
	case RPAREN: cout<<")"<<std::endl; break;
	case PLUSASSG:		cout<<"+="<<std::endl; break;
	case MINUASSG:		cout<<"-="<<std::endl; break;
	case TIMEASSG:		cout<<"*="<<std::endl; break;
	case OVERASSG:		cout<<"/="<<std::endl; break;
	case PLUSTIMEASSG:	cout<<"+*="<<std::endl; break;
	case PLUSOVERASSG:	cout<<"+/="<<std::endl; break;
	case MINUTIMEASSG:	cout<<"-*="<<std::endl; break;
	case MINUOVERASSG:	cout<<"-/="<<std::endl; break;
	case TIMEPLUSASSG:	cout<<"*+="<<std::endl; break;
	case TIMEMINUASSG:	cout<<"*-="<<std::endl; break;
	case OVERPLUSASSG:	cout<<"/+="<<std::endl; break;
	case OVERMINUASSG:	cout<<"/-="<<std::endl; break;
	case ENDFILE:		cout<<"EOF"<<std::endl; break;
	case ERROR: cout<<"Error: "<<currMsg<<std::endl; break;
	default:
		cout<<"Unknown Token Type: "<<type<<std::endl;
	break;
	}
}

std::string Lexay::getMsg(){
	return currMsg;
}

int Lexay::getMsgInt(){
	return atoi(currMsg.c_str());
}

void Lexay::testLex(){
	while (getToken() != ENDFILE);
}
