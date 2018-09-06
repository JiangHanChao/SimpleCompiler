#include <iostream>
#include <fstream>
#include <stack>
#include <cstdlib>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

// define of spm cosnt variable
const static int mem_size  = 4;
const static int stack_size = 1024*mem_size;
const static int regs_size = 8;
const static int insc_size = 14;

enum RegType {
	EAX=0, EBX, ECX, EDX, EBP, ESP, EIP, ENM, ENLL=8
};
enum INSCTYPE {
	MOV=0, LOD, ADD, SUB, MUL, DIV, SHF, SHR, JMP, LEA, RET, CALL, HALT, LEAVE=13
};
enum INTERRUPT {
	OUT=0
};
const char* const INSCTAB[] = {
	"MOV","LOD","ADD","SUB","MUL","DIV","SHF","SHR","JMP","LEA","RET","CALL","HALT","LEAVE"
};

typedef struct{
	INSCTYPE iop;
	int 	 inum1;
	RegType  ireg1;
	int 	 inum2;
	RegType  ireg2;
} INSTRUCTION;
// end of defines

class Spm {
public:
	bool error = false;
	Spm(){
		try{
			stack = new size_t[stack_size];
		}catch(const std::bad_alloc& e){
			std::cerr<<e.what()<<std::endl;
			std::cout<<"Initialize Spm stack memory error, Please try again."<<std::endl;
			exit(1);
		}
		
	}
	~Spm(){
		if (stack != NULL){
			delete[] stack;
			stack = NULL;
		}
		if (text != NULL){
			delete[] text;
			text = NULL;
		}
	}
	void callerr(void){
		error = true;
	}
	void load(ifstream& fin){
		string tmp;
		vector<string> v;

		getline(fin, tmp);
		if (match(tmp,"\t.file\t")){
			cout<<"loading file \'"<<tmp<<"\' ..."<<endl;
			filename = tmp;
		}else{
			callerr();
			return ;
		}

		getline(fin,tmp);
		if (match(tmp, "\t.comment")){
			eflag |= (1<<31);
			cout<<"enable code comment..."<<endl;
			getline(fin, tmp);
			while(1){
				if (match(tmp, "**\t")){
					cout<<tmp<<endl;
					getline(fin, tmp);
				}else
					break;
			}
		}
		
		while(std::getline(fin, tmp)){
			if (match(tmp, "**\t")){
				if (eflag&(1<<31))
					cout<<tmp<<endl;
				continue;
			}
			if(!tmp.empty() && tmp!="")
				v.push_back(tmp);
		}

		if(v.size() > 0){
			try{
				text = new INSTRUCTION[v.size()+1];
				text_size = v.size();
			}catch(const std::bad_alloc& e){
				cerr<<e.what()<<endl;
				cout<<"spm load text code memory error."<<endl;
			}
		}else{
			callerr();
			cout<<"file has no code run."<<endl;
			return ;
		}

		readINSTRCTION(v);
	}
	bool docmd(void){
		string input;
		std::getline(cin, input);
		if (input.length() != 1){
			cout<<"Unexcepted input command."<<endl<<endl;
			return true;
		}

		if (!input.compare("q")){
			return false;
		}
		return true;
	}
protected:
	bool match(string& src, string rule){
		if (src.empty() || rule.empty()){
			return false;
		}
		int len = src.length();
		int rlen = rule.length();
		if (len < rlen){
			return false;
		}
		int i = 0;
		for (; i<len && i<rlen; ++i){
			if (src[i]!=rule[i]){
				break;
			}
		}
		if (i == rlen){
			if (len == rlen){
				src = "";
			}else
				src = src.substr(i);
			return true;
		}else{
			return false;
		}
	}
	void readINSTRCTION(const vector<string>& content){
		int num1, num2;
		RegType reg1, reg2;
		for (int i = 0; i < text_size; ++i){
			string ins = content[i];
			int pos = atoi(ins.substr(0,3).c_str());
			ins = ins.substr(4);
			if (pos!=i+1){
				cout<<"read instruction position error"<<endl;
				callerr();
				return;
			}
			INSCTYPE t = lookType(readWord(ins));
			text[i].iop = t;
			if (t == RET || t == LEAVE){
				num1 = num2 = 0;
				reg1 = reg2 = EAX;
			}else if(t == CALL){
				match(ins, "CALL\t");
				if (ins.compare("OUT")==0){
					num1 = 0; reg1=ENM;
				}else
					callerr();
				num2 = 0;
				reg2 = ENLL;
			}else{
				match(ins, INSCTAB[(int)t]+'\t');
				size_t fpos = ins.find(',');
				if (ins[0]=='$'){
					num1 = atoi(ins.substr(1,fpos-1).c_str());
					reg1 = ENM;
				}else if(ins[0]=='%'){
					num1 = 0;
					reg1 = readReg(ins);
				}else{
					size_t lpos = ins.find('(');
					num1 = atoi(ins.substr(0,lpos).c_str());
					reg1 = readReg(ins, lpos);
				}

				ins = ins.substr(fpos+2);
				if (ins[0]=='$'){
					num2 = atoi(ins.substr(1).c_str());
					reg2 = ENM;
				}else if(ins[0]=='%'){
					num2 = 0;
					reg2 = readReg(ins);
				}else{
					size_t lpos = ins.find('(');
					num2 = atoi(ins.substr(0,lpos).c_str());
					reg2 = readReg(ins, lpos);
				}
			}

			text[i].inum1 = num1;
			text[i].inum2 = num2;
			text[i].ireg1 = reg1;
			text[i].ireg2 = reg2;
		}
	}
	INSCTYPE lookType(const string& type){
		for (int i = 0; i < insc_size; ++i){
			if(type.compare(INSCTAB[i])==0)
				return INSCTYPE(i);
		}
		callerr();
		cout<<"instruction type no found."<<endl;
		return RET;
	}
	RegType readReg(const string& ins,int lpos=-1){
		if (ins[lpos+1]=='%'&&ins[lpos+2]=='e'){
			switch(ins[lpos+3]){
			case 'a':
			if(ins[lpos+4]=='x')return EAX;
			break;
			case 'b':
			if(ins[lpos+4]=='x')return EBX;
			else if(ins[lpos+4]=='p')return EBP;
			break;
			case 'c':
			if(ins[lpos+4]=='x')return ECX;
			break;
			case 'd':
			if(ins[lpos+4]=='x')return EDX;
			break;
			case 's':
			if(ins[lpos+4]=='p')return ESP;
			break;
			default:
			callerr();
			cout<<"regeister read error"<<endl;
			return EAX;
			}
		}
		return ENM;
	}
	string readWord(const string& ins){
		int i = 0;
		int len = ins.size();
		while(i!=len && ins[i]!='\t')
			++i;
		if (i==len)
			return ins;
		else
			return ins.substr(0,i);
	}
	void call_interrupt(INTERRUPT sgm){
		switch(sgm){
		case OUT:
			//TODO cout %esp value
		break;
		default:
		break;
		}
	}
private:
	size_t* stack = NULL;
	INSTRUCTION* text = NULL;
	size_t  eip  = 0;
	size_t  eflag= 0;
	size_t  text_size = 0;
	size_t  regs[regs_size] = {0};
	string filename;
};

int main(int argc, char const *argv[])
{
	Spm spm;
	if (argc != 2){
		cout<<"usage: "<<argv[0]<<" <filename>"<<endl;
		return 1;
	}
	string name = argv[1];

	if (name.find(".sm")==string::npos){
		cout<<"File \'"<<name<<"\' can't be executable, formatter is error."<<endl;
		return 4;
	}

	ifstream fp;
	fp.open(name.c_str());
	if (fp.bad()){
		cout<<"File \'"<<name<<"\' no found. Please check name."<<endl;
		return 2;
	}

	spm.load(fp);
	if (spm.error){
		cout<<"load file code error."<<endl;
		return 3;
	}

	cout<<endl<<"...SPM simulation start..."<<endl;
	cout<<">>> (enter h for cmd help)"<<endl;
	bool done = true;
	do{
		cout<<">>>";
		done= spm.docmd();
	}while(done);

	cout<<"SPM shutdown.\nBye!"<<endl;
	fp.close();
	return 0;
}