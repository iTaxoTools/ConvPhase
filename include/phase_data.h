#ifndef PHASE_DATA_H
#define PHASE_DATA_H
#include <string>
#include <iostream>

struct PhaseInput{
	PhaseInput() = default;
	PhaseInput(std::string input):
		input{input}{
	}
	PhaseInput(std::string input, std::string known):
		input{input},
		known{known}{
		}
	PhaseInput(std::string input, std::string known, std::string init, std::string delta):
		input{input},
		known{known},
		init{init},
		delta{delta}{
	}

	std::string input;

	std::string known;
	std::string init;
	std::string delta;
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
		known = in.known;
		init  = in.init;
		delta = in.delta;
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

	std::string known;
	std::string init;
	std::string delta;

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
