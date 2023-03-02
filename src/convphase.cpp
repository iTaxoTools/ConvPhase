#include "convphase.h"
#include "hxcpp_symbols.h"
#include "phase.h"

#include "SeqPhase1.h"
#include "SeqPhase2.h"

#include <cassert>

//extern "C" void hxcpp_set_top_of_stack();
//extern "C" char const* hxRunLibrary();
int oldArgc;
char** oldArgv;

void initHxcpp(){
	HX_TOP_OF_STACK
	hx::Boot();
	__boot_all();

	//hxcpp_set_top_of_stack();
	//hxRunLibrary();
}
void seqPhaseStep1(char const* file1, char const* file2, char const* file3){
	std::vector<char const*> args{"", "-1", file1};
	if(file2){
		args.push_back("-2");
		args.push_back(file2);
	}
	if(file3){
		args.push_back("-3");
		args.push_back(file3);
	}
	int argc = args.size();
	char** argv = createArgArray(args);

	setHxcppArgs(argc, argv);
	SeqPhase1_obj::main();
	restoreHxcppArgs();

	deleteArgArray(argv, args.size());
}
void seqPhaseStep1_2(char const* file1, char const* file2, char const* file3){
	SeqPhase1Result result = SeqPhase1_obj::doIt(
		String::create(file1, strlen(file1)),
		file2 ? String::create(file2, strlen(file2)) : null(),
		file3 ? String::create(file3, strlen(file3)) : null()
	);
}
void seqPhaseStep2(char const* constFile, char const* phaseFile){
	std::vector<char const*> args{"", "-c", constFile, "-i", phaseFile};
	int argc = args.size();
	char** argv = createArgArray(args);

	setHxcppArgs(argc, argv);
	SeqPhase2_obj::main();
	restoreHxcppArgs();

	deleteArgArray(argv, args.size());
	_hxcpp_argc = oldArgc;
	_hxcpp_argv = oldArgv;
}

int phase(char const* inputFile, char const* outputFile, std::vector<char const*> options){
	std::vector<char const*> args{""};
	args.insert(args.end(), options.begin(), options.end());
	//args.push_back("-p0.3");
	args.push_back(inputFile);
	args.push_back(outputFile);
	int argc = args.size();
	char** argv = createArgArray(args);

	int ret = phase(argc, argv);

	deleteArgArray(argv, args.size());
	return ret;
}

int convPhase(char const* inputFile, std::vector<char const*> options){
	initHxcpp();
	seqPhaseStep1(inputFile);
	phase("phase.inp", "phase.out", options);
	seqPhaseStep2();

	return 0;
}

char** createArgArray(std::vector<char const*> args){
	char** argArr = new char*[args.size()];
	for(int i = 0; i < args.size(); ++i){
		argArr[i] = new char[strlen(args[i])+1];
		strcpy(argArr[i], args[i]);
	}
	return argArr;
}
void deleteArgArray(char** argArr, int count){
	for(int i = 0; i < count; ++i)
		delete[] argArr[i];
	delete[] argArr;
}
void setHxcppArgs(int argc, char** argv){
	oldArgc = _hxcpp_argc;
	oldArgv = _hxcpp_argv;
	_hxcpp_argc = argc;
	_hxcpp_argv = argv;
}
void restoreHxcppArgs(){
	_hxcpp_argc = oldArgc;
	_hxcpp_argv = oldArgv;
}
