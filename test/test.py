#!/usr/bin/env python
import convphase
import sys

file = open(sys.argv[1], "r")
inp = file.read()

#out = convphase.convPhase(inp, "-p0.3")

inpData, knownData, constData = convphase.seqPhaseStep1(inp)
phaseOut = convphase.phase(inpData, knownData, "-p0.3")
out = convphase.seqPhaseStep2(phaseOut, constData)

print(out)
