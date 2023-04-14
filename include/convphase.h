#ifndef CONVPHASE_H
#define CONVPHASE_H

#ifdef _MSC_VER
#define CONVPHASE_API __declspec(dllexport)
#else
#define CONVPHASE_API extern "C"
#endif

#include "phase.h"

#include <vector>
#include <string>

struct SeqPhaseStep1Result{
	std::string inpData;
	std::string knownData;
	std::string constData;
};

void initHxcpp(); //Must be called before seqPhaseStep1 and seqPhaseStep2

CONVPHASE_API SeqPhaseStep1Result seqPhaseStep1(std::string str1, std::string str2 = "", std::string str3 = "");
CONVPHASE_API PhaseOutput phase(PhaseInput input, std::vector<char const*> options = std::vector<char const*>{});
CONVPHASE_API std::string seqPhaseStep2(
	std::string phaseFile, std::string constFile = "",
	bool reduce = false, bool sort = false
);
CONVPHASE_API std::string convPhase(
	std::string input, std::vector<char const*> options = std::vector<char const*>{},
	bool reduce = false, bool sort = false
);


char** createArgArray(std::vector<char const*> args); //Allocations have to be freed with deleteArgArray
void deleteArgArray(char** argArr, int count);
void setHxcppArgs(int argc, char** argv);  //set args for hxcpp and save old value
void restoreHxcppArgs();  //restore args for hxcpp
std::string readFile(char const* filename);
#endif
