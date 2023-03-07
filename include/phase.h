#ifndef PHASE_H
#define PHASE_H

#include "phase_data.h"

#include <iostream>

#ifdef CP_PHASE_NOFILE
int phase(PhaseData& data, int argc, char* argv[]);
#else
int phase(int argc, char* argv[]);
#endif

#endif
