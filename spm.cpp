#include <iostream>
#include <stack>
#include <cstdlib>
#include <vector>
#include <string>
#include <algorithm>

// define of spm cosnt variable
const int mem_size  = 4;
const int heap_size = 1024*1024*mem_size;

// end of defines

class Spm {
public:
	Spm(){
		try{
			heap = new size_t[heap_size];
		}catch(const std::bad_alloc& e){
			std::cerr<<e.what()<<std::endl;
			std::cout<<"Initialize Spm error, Please try again."<<std::endl;
			exit(1);
		}
		
	}
	~Spm(){
		if (heap != NULL){
			delete[] heap;
		}
	}
private:
	std::stack<size_t> s;
	size_t* heap;

};

int main(int argc, char const *argv[])
{
	Spm spm;

	return 0;
}