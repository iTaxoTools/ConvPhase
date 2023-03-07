#ifndef PHASE_DATA_H
#define PHASE_DATA_H
#include <string>
#include <iostream>

struct PhaseInput{
	PhaseInput() = default;
	PhaseInput(std::string in){
		input = in;
	}

	std::string input;

	std::string delta;
	std::string known;
	std::string init;
};
struct PhaseOutput{
	std::string output;

	std::string freqs;
	std::string monitor;
	std::string hbg;
	std::string probs;

	std::string recom;
	std::string sample;
	std::string pairs;
	std::string signif;
	std::string hot;

	std::string cout;
	std::string cerr;
};
struct PhaseData{
	PhaseData() = default;
	PhaseData(PhaseInput in){
		input = in.input;
		delta = in.delta;
		known = in.known;
		init  = in.init;
	}

	PhaseOutput getOutput(){
		return PhaseOutput{
			output,

			freqs,
			monitor,
			hbg,
			probs,

			recom,
			sample,
			pairs,
			signif,
			hot,

			cout,
			cerr
		};
	}

	std::string input;
	std::string output;

	std::string delta;
	std::string known;
	std::string init;

	std::string freqs;
	std::string monitor;
	std::string hbg;
	std::string probs;

	std::string recom;
	std::string sample;
	std::string pairs;
	std::string signif;
	std::string hot;

	std::string cout;
	std::string cerr;
};
#endif
