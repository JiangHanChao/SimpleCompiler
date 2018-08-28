#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

#include "compiler.h"

int main(int argc, char** argv) {
	using namespace std;
	short printoken = 0; // 1; show source anaylze token
	short printree  = 1; // 0; show source anaylsis tree
	short printable = 1; // 0; show variable symbol table
	short printrace = 1; // 0; show destination code detail trace
	short enableInfo = 1; // 0; wirte infomation to destination code
	int flag = (printoken<<8)+(printree<<9)+(printable<<10)+(printrace<<11)+(enableInfo<<12);
	ifstream fp;
	string codename;
	string filename = argv[1];
	SimpleCompiler compiler;
	
	if(argc!=2){
		cout<<"usage: "<<argv[0]<<" <filename>"<<endl;
		exit(1);
	}
	
	if(filename.find(".") == string::npos){
		codename = filename+".sm";
		filename += ".sp";
	}else{
		codename = filename.substr(0, filename.find(".")+1)+"sm";
	}
	
	fp.open(filename.c_str(), std::ios_base::in);
	if(fp.fail()){
		cout<<"File <"<<filename<<"> not found."<<endl;
		exit(2);
	}
	cout<<"Simple Compiler Compilation: "<<filename<<endl;

	compiler.setSource(&fp);
	compiler.setFlag(flag);
	compiler.setCodename(codename);

	compiler.run();
	
	return 0;
}
