#include <cstdio>

#include "convphase.h"

int main(int argc, char* argv[]){
	if(argc <= 1)
		fprintf(stderr, "Error: No input file given\n");
	char const* inputFile = argv[1];

	std::vector<char const*> options{};
	for(int i = 2; i < argc; ++i)
		options.push_back(argv[i]);

#ifdef CP_PHASE_NOFILE
	std::string result = convPhase(readFile(inputFile), options);
	printf("%s\n", result.c_str());
#else
	convPhase(inputFile, options);
#endif

	return 0;
}
