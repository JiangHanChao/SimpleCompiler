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
const static int regs_size = 6;

enum RegType {
	EAX=0, EBX, ECX, EDX, EBP, ESP=5
};
enum INSCTYPE {
	MOV=0, ADD, SUB, MUL, DIV, SHF, SHR, JMP, LEA, RET, LEAVE=10
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
	void load(ifstream& fin){
		string tmp;
		while(std::getline(fin, tmp)){
			if(!tmp.empty() && tmp!="")
				text_size++;
		}
		if(text_size > 0){
			try{
				text = new INSTRUCTION[text_size+1];
			}catch(const std::bad_alloc& e){
				cerr<<e.what()<<endl;
				cout<<"spm load text code memory error."<<endl;
			}
		}

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
private:
	size_t* stack = NULL;
	INSTRUCTION* text = NULL;
	size_t  eip  = 0;
	size_t  eflag= 0;
	size_t  text_size = 0;
	size_t  regs[regs_size] = {0};
};

int main(int argc, char const *argv[])
{
	Spm spm;
	if (argc != 2){
		cout<<"usage: "<<argv[0]<<" <filename>"<<endl;
		return 1;
	}
	string name = argv[1];

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

	cout<<"SPM simulation start..."<<endl;
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