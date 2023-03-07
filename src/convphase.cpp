#include "convphase.h"
#include "hxcpp_symbols.h"

#include "SeqPhase1.h"
#include "SeqPhase1Result.h"
#include "SeqPhase2.h"
#include "Individuals.h"

#include <iostream>
#include <sstream>
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

#ifdef CP_PHASE_NOFILE
SeqPhaseStep1Result seqPhaseStep1(std::string str1, std::string str2, std::string str3){
//	SeqPhase1Result result = SeqPhase1_obj::doIt(
//		String::create(str1, strlen(str1)),
//		file2 ? String::create(file2, strlen(file2)) : null(),
//		file3 ? String::create(file3, strlen(file3)) : null()
//	);
	SeqPhase1Result result = SeqPhase1_obj::doIt(
		String::create(str1.c_str(), str1.size()),
		str2.size() ? String::create(str2.c_str(), str2.size()) : null(),
		str3.size() ? String::create(str3.c_str(), str3.size()) : null()
	);
	assert(!result->hasErrors());

	SeqPhaseStep1Result data;
	if(result->hasErrors()){
		fprintf(stderr, "Error: SeqPhaseStep1 failed!\n");
		return data;
	}
	if(result->hasInpFile())
		data.inpData = result->getInpFile().c_str();
	if(result->hasKnownFile())
		data.knownData = result->getKnownFile().c_str();
	if(result->hasConstFile())
		data.constData = result->getConstFile().c_str();
	return data;
}
PhaseOutput phase(PhaseInput input, std::vector<char const*> options){
	std::vector<char const*> args{""};
	args.insert(args.end(), options.begin(), options.end());
	int argc = args.size();
	char** argv = createArgArray(args);

	PhaseData data{input};
	int ret = phase(data, argc, argv);
	assert(!ret);

	deleteArgArray(argv, args.size());
	return data.getOutput();
}
std::string seqPhaseStep2(std::string phaseFile, std::string constFile, bool reduce, bool sort){
	Individuals result = SeqPhase2_obj::parse(
		String::create(phaseFile.c_str(), phaseFile.size()),
		String::create(constFile.c_str(), constFile.size())
	);
	String fileContent = result->getFasta(reduce, sort);
	return std::string(fileContent.c_str());
}
std::string convPhase(std::string input, std::vector<char const*> options, bool reduce, bool sort){
	initHxcpp();

	SeqPhaseStep1Result step1 = seqPhaseStep1(input);
	//printf("%s\n", step1.inpData.c_str());

	PhaseOutput phaseResult = phase(PhaseInput{step1.inpData}, options);
	//printf("%s\n", phaseResult.output.c_str());
	//printf("%s\n", phaseResult.pairs.c_str());
	printf("freqs: \n%s\n\n\n", phaseResult.freqs.c_str());
	printf("monitor: \n%s\n\n\n", phaseResult.monitor.c_str());
	printf("hbg: \n%s\n\n\n", phaseResult.hbg.c_str());
	printf("probs: \n%s\n\n\n", phaseResult.probs.c_str());

	printf("recom: \n%s\n\n\n", phaseResult.recom.c_str());
	printf("sample: \n%s\n\n\n", phaseResult.sample.c_str());
	printf("pairs: \n%s\n\n\n", phaseResult.pairs.c_str());
	printf("signif: \n%s\n\n\n", phaseResult.signif.c_str());
	printf("hot: \n%s\n\n\n", phaseResult.hot.c_str());

	std::string step2 = seqPhaseStep2(phaseResult.output, step1.constData, reduce, sort);
	//printf("%s\n", step2.c_str());

	return step2;
}
#else
void seqPhaseStep1Main(char const* file1, char const* file2, char const* file3){
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
int phase(char const* inputFile, char const* outputFile, std::vector<char const*> options){
	std::vector<char const*> args{""};
	args.insert(args.end(), options.begin(), options.end());
	args.push_back(inputFile);
	args.push_back(outputFile);
	int argc = args.size();
	char** argv = createArgArray(args);

	int ret = phase(argc, argv);

	deleteArgArray(argv, args.size());
	return ret;
}
void seqPhaseStep2Main(char const* phaseFile, char const* constFile){
	std::vector<char const*> args{"", "-i", phaseFile, "-c", constFile};
	int argc = args.size();
	char** argv = createArgArray(args);

	setHxcppArgs(argc, argv);
	SeqPhase2_obj::main();
	restoreHxcppArgs();

	deleteArgArray(argv, args.size());
	_hxcpp_argc = oldArgc;
	_hxcpp_argv = oldArgv;
}
int convPhase(char const* inputFile, std::vector<char const*> options){
	initHxcpp();

	seqPhaseStep1Main(inputFile);

	phase("phase.inp", "phase.out", options);

	seqPhaseStep2Main();

	return 0;
}
#endif

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
std::string readFile(char const* filename){
	FILE* file = fopen(filename, "r");
	assert(file);
	fseek(file, 0, SEEK_END);
	long len = ftell(file);
	rewind(file);

	char* str = new char[len+1];
	fread(str, 1, len, file);
	str[len] = 0;

	fclose(file);
	return str;
}
