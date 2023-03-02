#include <cstdio>

#include "convphase.h"

int main(int argc, char* argv[]){
	printf("Hello World!\n");

	if(argc <= 1)
		fprintf(stderr, "Error: No input file given\n");
	char const* inputFile = argv[1];

	std::vector<char const*> options{};
	for(int i = 2; i < argc; ++i)
		options.push_back(argv[i]);

	convPhase(inputFile, options);
	//initHxcpp();
	//seqPhaseStep1(inputFile, inputFile, inputFile);

	return 0;
}
