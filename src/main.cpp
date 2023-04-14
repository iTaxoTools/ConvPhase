#include <cstdio>
#include <cassert>

#include "convphase.h"

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

int main(int argc, char* argv[]){
	if(argc <= 1)
		fprintf(stderr, "Error: No input file given\n");
	char const* inputFile = argv[1];

	std::vector<char const*> options{};
	for(int i = 2; i < argc; ++i)
		options.push_back(argv[i]);

	std::string result = convPhase(readFile(inputFile), options);
	printf("%s\n", result.c_str());

	return 0;
}
