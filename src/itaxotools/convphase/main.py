from __future__ import annotations

from .convphase import convPhase, _iterator_test


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
    input: iter[tuple[str, str]],
    number_of_iterations: int = 100,
    thinning_interval: int = 1,
    burn_in: int = 100,
    phase_threshold: float = 0.9,
    allele_threshold: float = 0.9,
) -> iter[tuple[str, str, str]]:

    args = []

    args.append(f'-p{phase_threshold}')
    args.append(f'-q{allele_threshold}')

    args.append(str(number_of_iterations))
    args.append(str(thinning_interval))
    args.append(str(burn_in))

    output = _iterator_test(input)
    return output
