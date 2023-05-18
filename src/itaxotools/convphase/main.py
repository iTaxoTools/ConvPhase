from __future__ import annotations

from .convphase import convPhase, iterPhase
from .types import PhasedSequence, UnphasedSequence


def main(
    input: str,
    number_of_iterations: int = 100,
    thinning_interval: int = 1,
    burn_in: int = 100,
    phase_threshold: float = 0.9,
    allele_threshold: float = 0.9,
) -> str:

    args = []

    args.append(input)

    args.append(f'-p{phase_threshold}')
    args.append(f'-q{allele_threshold}')

    args.append(str(number_of_iterations))
    args.append(str(thinning_interval))
    args.append(str(burn_in))

    output = convPhase(*args)
    return output


def iter_phase(
    input: iter[UnphasedSequence],
    number_of_iterations: int = 100,
    thinning_interval: int = 1,
    burn_in: int = 100,
    phase_threshold: float = 0.9,
    allele_threshold: float = 0.9,
) -> iter[PhasedSequence]:

    args = []

    args.append(f'-p{phase_threshold}')
    args.append(f'-q{allele_threshold}')

    args.append(str(number_of_iterations))
    args.append(str(thinning_interval))
    args.append(str(burn_in))

    output = iterPhase(input, args)
    return (PhasedSequence(*x) for x in output)
