#ifndef CONVPHASE_H
#define CONVPHASE_H

#include "phase.h"

#include <vector>
#include <string>

struct SeqPhaseStep1Result{
	std::string inpData;
	std::string knownData;
	std::string constData;
};

void initHxcpp(); //Must be called before seqPhaseStep1 and seqPhaseStep2

#ifdef CP_PHASE_NOFILE
SeqPhaseStep1Result seqPhaseStep1(std::string str1, std::string str2 = "", std::string str3 = "");
PhaseOutput phase(PhaseInput input, std::vector<char const*> options = std::vector<char const*>{});
std::string seqPhaseStep2(
	std::string phaseFile, std::string constFile = "",
	bool reduce = false, bool sort = false
);
std::string convPhase(
	std::string input, std::vector<char const*> options = std::vector<char const*>{},
	bool reduce = false, bool sort = false
);
#else
void seqPhaseStep1Main(char const* file1, char const* file2 = nullptr, char const* file3 = nullptr); //Output in files phase.inp and phase.const
int phase(char const* inputFile = "phase.inp", char const* outputFile = "phase.out", std::vector<char const*> options = std::vector<char const*>{});
void seqPhaseStep2Main(char const* phaseFile = "phase.out", char const* constFile = "phase.const"); //Main output in file phased.fasta, also writing to phase.out_{freqs,hbg,monitor,pairs,probs,recom}
int convPhase(char const* inputFile, std::vector<char const*> options = std::vector<char const*>{});
#endif


char** createArgArray(std::vector<char const*> args); //Allocations have to be freed with deleteArgArray
void deleteArgArray(char** argArr, int count);
void setHxcppArgs(int argc, char** argv);  //set args for hxcpp and save old value
void restoreHxcppArgs();  //restore args for hxcpp
std::string readFile(char const* filename);
#endif
