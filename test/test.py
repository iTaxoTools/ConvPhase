#!/usr/bin/env python
import libconvphase
import sys

file = open(sys.argv[1], "r")
inp = file.read()

#out = libconvphase.convPhase(inp, "-p0.3")

inpData, knownData, constData = libconvphase.seqPhaseStep1(inp)
phaseOut = libconvphase.phase(inpData, knownData, "-p0.3")
out = libconvphase.seqPhaseStep2(phaseOut, constData)

print(out)
