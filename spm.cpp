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
const char* const REGTAB[] = {
	"%eax","%ebx","%ecx","%edx","%ebp","%esp","%eip","num","NULL"
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
		RegType reg;
		int len;
		std::getline(cin, input);
		if (input[0]!='p' && input.length() != 1){
			cout<<"Unexcepted input command."<<endl<<endl;
			return true;
		}
		char op = input[0];
		switch(op){
		case 'h':
			cout<<"cmd :"<<endl;
			cout<<"h	help: for command list"<<endl;
			cout<<"q	quit: exit the SPM simulation"<<endl;
			cout<<"r	run : run the code in the simulation"<<endl;
			cout<<"n/s	next/setp: run one instruction in SPM"<<endl;
			cout<<"p	print: watch the regiest value by name. Exp: p %eax"<<endl;
			break;
		case 'r':
			runAll();
		case 'q':
			return false;
		case 'n':
		case 's':
			run();
			break;
		case 'p':
			len=input.length()-1;
			for(int i=1; i<len; i++)
				if(input[i]==' '&&input[i+1]!=' '){
					input = input.substr(i+1);
					break;
				}else if(i==len-1){
					cout<<"'p' arguments not given."<<endl;
					return true;
				}
			reg = readReg(input);
			cout<<input;
			if(error){
				cout<<" not a vaild regeister."<<endl;
				error = false;
			}else{
				printReg(reg);
			}
			break;
		default:
			cout<<"command '"<<op<<"' not found. input 'h' for help."<<endl;
			break;
		}
		return ret;
	}
protected:
	void checkFlag(){
		if(eflag&0x8){
			if(!(eflag&0x1))cout<<"exit uncorrect..."<<endl;
			ret = false;
		}
	}
	void printReg(RegType tp){
		cout<<" :"<<regs[tp]<<endl;
	}
	void runAll(){
		while(ret) run();
	}
	void run(){
		INSTRUCTION curr = text[eip];
		if(pause) execute(curr);
		else{
			if(eip==0){
				if(curr.iop==MOV && curr.ireg2==EAX){
					eflag = 1;
					execute(curr);
				}
			}else if(eip==1){
				if(curr.iop==LOD && curr.ireg1==EIP){
					pause = true;
				}
			}else if(eip+1==text_size){
				execute(curr);
			}
		}
		checkFlag();
		eip++;
	}
	void execute(INSTRUCTION ins){
		if(eip >= text_size){
			cout<<"EIP "<<eip<<": over than code text length."<<endl;
			eflag = 8;
			return ;
		}
		switch(ins.iop){
		case MOV:
			if(ins.ireg1==ENM){
				if(ins.ireg2 <= EDX){
					regs[ins.ireg2] = ins.inum1;
				}else if(ins.ireg2 <= ESP){
					stack[regs[ins.ireg2]+ins.inum2] = ins.inum1;
				}else if(ins.ireg2==EIP){
					eip = ins.inum1;
				}else{
					eflag=0;
				}
			}else if(ins.ireg1 <= EDX){
				if(ins.ireg2 <= EDX)
					regs[ins.ireg2] = regs[ins.ireg1];
				else if(ins.ireg2 <= ESP){
					stack[regs[ins.ireg2]+ins.inum2] = regs[ins.ireg1];
				}else
					eip = regs[ins.ireg1];
			}else if(ins.ireg1 <= ESP){
				if(ins.ireg2 <= EDX)
					regs[ins.ireg2] = stack[regs[ins.ireg1]+ins.inum1];
				else if(ins.ireg2 <= ESP)
					stack[regs[ins.ireg2]+ins.inum2] = stack[regs[ins.ireg1]+ins.inum1];
				else
					eip = stack[regs[ins.ireg1]+ins.inum1];
			}else
				eflag = 0;
			break;
		case LOD:
			if(ins.ireg1 != EAX)
				regs[ins.ireg1] = regs[EAX];
			else
				eflag = 0;
			break;
		case ADD:
			if(ins.ireg2==EBP || ins.ireg2==ESP){
				if(ins.ireg1 <= EDX)
					stack[regs[ins.ireg2]+ins.inum2] += regs[ins.ireg1];
				else if(ins.ireg1 <= ESP)
					stack[regs[ins.ireg2]+ins.inum2] += stack[regs[ins.ireg1]+ins.inum1];
				else
					eflag = 0;
			}else if(ins.ireg2 <= EDX){
				if(ins.ireg1 <= EDX)
					regs[ins.ireg2] += regs[ins.ireg1];
				else if(ins.ireg1 <= ESP)
					regs[ins.ireg2] += stack[regs[ins.ireg1]+ins.inum1];
				else
					eflag = 0;
			}else
				eflag = 0;
			break;
		case SUB:
			if(ins.ireg2==EBP || ins.ireg2==ESP){
				if(ins.ireg1 <= EDX)
					stack[regs[ins.ireg2]+ins.inum2] -= regs[ins.ireg1];
				else if(ins.ireg1 <= ESP)
					stack[regs[ins.ireg2]+ins.inum2] -= stack[regs[ins.ireg1]+ins.inum1];
				else
					eflag = 0;
			}else if(ins.ireg2 <= EDX){
				if(ins.ireg1 <= EDX)
					regs[ins.ireg2] -= regs[ins.ireg1];
				else if(ins.ireg1 <= ESP)
					regs[ins.ireg2] -= stack[regs[ins.ireg1]+ins.inum1];
				else
					eflag = 0;
			}else
				eflag = 0;
			break;
		case MUL:
			if(ins.ireg2==EBP || ins.ireg2==ESP){
				if(ins.ireg1 <= EDX)
					stack[regs[ins.ireg2]+ins.inum2] *= regs[ins.ireg1];
				else if(ins.ireg1 <= ESP)
					stack[regs[ins.ireg2]+ins.inum2] *= stack[regs[ins.ireg1]+ins.inum1];
				else
					eflag = 0;
			}else if(ins.ireg2 <= EDX){
				if(ins.ireg1 <= EDX)
					regs[ins.ireg2] *= regs[ins.ireg1];
				else if(ins.ireg1 <= ESP)
					regs[ins.ireg2] *= stack[regs[ins.ireg1]+ins.inum1];
				else
					eflag = 0;
			}else
				eflag = 0;
			break;
		case DIV:
			if(ins.ireg2==EBP || ins.ireg2==ESP){
				if(ins.ireg1 <= EDX){
					if(regs[ins.ireg1] == 0){
						cout<<"DIV : zero divide overflow error."<<endl;
						eflag = 0;
					}else
						stack[regs[ins.ireg2]+ins.inum2] /= regs[ins.ireg1];
				}else if(ins.ireg1 <= ESP){
					if(stack[regs[ins.ireg1]+ins.inum1]==0){
						cout<<"DIV : zero divide overflow error."<<endl;
						eflag = 0;
					}else
						stack[regs[ins.ireg2]+ins.inum2] /= stack[regs[ins.ireg1]+ins.inum1];
				}
				else
					eflag = 0;
			}else if(ins.ireg2 <= EDX){
				if(ins.ireg1 <= EDX){
					if(regs[ins.ireg1]==0){
						cout<<"DIV : zero divide overflow error."<<endl;
						eflag = 0;
					}else
						regs[ins.ireg2] /= regs[ins.ireg1];
				}else if(ins.ireg1 <= ESP){
					if(stack[regs[ins.ireg1]+ins.inum1]==0){
						cout<<"DIV : zero divide overflow error."<<endl;
						eflag = 0;
					}else
						regs[ins.ireg2] /= stack[regs[ins.ireg1]+ins.inum1];
				}else
					eflag = 0;
			}else
				eflag = 0;
			break;
		case SHF:
		case SHR:
		case JMP:
		case LEA:
			break;
		case LEAVE:
		case RET:
			if(ins.ireg1==ENLL && ins.ireg2==ENLL){
				regs[EBP] = regs[ESP] = 0;
				regs[EAX] = 0;
				pause = false;
			}else
				eflag = 0;
			break;
		case CALL:
			call_interrupt(OUT);
			break;
		case HALT:
			eip = 0;
			eflag = 9;
			break;
		default:
			eflag = 1;
			break;
		}
		cout<<"EIP "<<eip<<": ";
		if(eflag&0x1){
			if(ins.iop==RET || ins.iop==LEAVE)cout<<INSCTAB[ins.iop]<<endl;
			else
				cout<<INSCTAB[ins.iop]<<" "<<ins.inum1<<"("<<REGTAB[ins.ireg1]<<"), "
					<<ins.inum2<<"("<<REGTAB[ins.ireg2]<<")"<<endl;
		}else{
			cout<<"instruction run error result."<<endl;
			eflag = 1;
		}
	}
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
				reg1 = reg2 = ENLL;
			}else if(t == CALL){
				match(ins, "CALL\t");
				if (ins.compare("OUT")==0){
					num1 = 0; reg1=ENM;
				}else
					callerr();
				num2 = 0;
				reg2 = ENLL;
			}else if(t == LOD){
				match(ins, "LOD\t");
				reg1 = readReg(ins);
				num1 = num2 = 0;
				reg2 = ENLL;
			}else{
				string rule=INSCTAB[(int)t];
				rule+="\t";
				match(ins, rule);
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
			case 'i':
			if(ins[lpos+4]=='p')return EIP;
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
			cout<<"out: "<<stack[regs[ESP]]<<endl<<endl;
		break;
		default:
		break;
		}
	}
private:
	bool ret = true;
	bool pause = false;
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
