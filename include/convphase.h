#ifndef CONVPHASE_H
#define CONVPHASE_H

#include <vector>

void initHxcpp(); //Must be called before seqPhaseStep1 and seqPhaseStep2
void seqPhaseStep1(char const* file1, char const* file2 = nullptr, char const* file3 = nullptr); //Output in files phase.inp and phase.const
void seqPhaseStep1_2(char const* file1, char const* file2 = nullptr, char const* file3 = nullptr);
void seqPhaseStep2(char const* constFile = "phase.const", char const* phaseFile = "phase.out"); //Main output in file phased.fasta, also writing to phase.out_{freqs,hbg,monitor,pairs,probs,recom}

int phase(char const* inputFile = "phase.inp", char const* outputFile = "phase.out", std::vector<char const*> options = std::vector<char const*>{});

int convPhase(char const* inputFile, std::vector<char const*> options = std::vector<char const*>{});

char** createArgArray(std::vector<char const*> args); //Allocations have to be freed with deleteArgArray
void deleteArgArray(char** argArr, int count);
void setHxcppArgs(int argc, char** argv);  //set args for hxcpp and save old value
void restoreHxcppArgs();  //resotre args for hxcpp
#endif
