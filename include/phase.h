#ifndef PHASE_H
#define PHASE_H

#include <iostream>

#ifdef CP_PHASE_NOFILE
int phase(std::istream& in, std::ostream& out, int argc, char* argv[]);
#else
int phase(int argc, char* argv[]);
#endif

#endif
