from __future__ import annotations

from pathlib import Path

from itaxotools.taxi2.sequences import Sequences, Sequence

from .convphase import convPhase, iterPhase
from .files import get_info_from_path, get_handler_from_info
from .types import PhasedSequence, UnphasedSequence


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


def phase_mimic_format(input_path: Path, output_path: Path, *args, **kwargs):

    info = get_info_from_path(input_path)
    read_handler_original = get_handler_from_info(input_path, 'r', info)
    read_handler_for_phasing = get_handler_from_info(input_path, 'r', info)
    write_handler = get_handler_from_info(output_path, 'w', info)

    unphased = Sequences(read_handler_for_phasing)
    unphased = (UnphasedSequence(sequence.id, sequence.seq) for sequence in unphased)
    phased = iter_phase(unphased, *args, **kwargs)

    original_data = Sequences(read_handler_original)

    with write_handler as file:
        for sequence, line in zip(original_data, phased):
            if sequence.id != line.id:
                raise Exception('Mismatch of ids between original and phased data')
            sequence_a = Sequence(line.id + 'a', line.data_a, sequence.extras)
            sequence_b = Sequence(line.id + 'b', line.data_b, sequence.extras)
            file.write(sequence_a)
            file.write(sequence_b)


def direct_phase(
    input: str,
    number_of_iterations: int = 100,
    thinning_interval: int = 1,
    burn_in: int = 100,
    phase_threshold: float = 0.9,
    allele_threshold: float = 0.9,
) -> str:

    """Uses the C++ backend to parse and write data"""

    args = []

    args.append(input)

    args.append(f'-p{phase_threshold}')
    args.append(f'-q{allele_threshold}')

    args.append(str(number_of_iterations))
    args.append(str(thinning_interval))
    args.append(str(burn_in))

    output = convPhase(*args)
    return output
