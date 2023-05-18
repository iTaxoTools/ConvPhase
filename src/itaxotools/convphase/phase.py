from pathlib import Path

from itaxotools.taxi2.sequences import Sequences, Sequence

from itaxotools.convphase.files import get_info_from_path, get_handler_from_info
from itaxotools.convphase.types import UnphasedSequence, UnphasedSequence
from itaxotools.convphase.main import iter_phase


def phase_mimic_format(input_path: Path, output_path: Path):

    info = get_info_from_path(input_path)
    read_handler_original = get_handler_from_info(input_path, 'r', info)
    read_handler_for_phasing = get_handler_from_info(input_path, 'r', info)
    write_handler = get_handler_from_info(output_path, 'w', info)

    unphased = Sequences(read_handler_for_phasing)
    unphased = (UnphasedSequence(sequence.id, sequence.seq) for sequence in unphased)
    phased = iter_phase(unphased)

    original_data = Sequences(read_handler_original)

    with write_handler as file:
        for sequence, line in zip(original_data, phased):
            if sequence.id != line.id:
                raise Exception('Mismatch of ids between original and phased data')
            sequence_a = Sequence(line.id + 'a', line.data_a, sequence.extras)
            sequence_b = Sequence(line.id + 'b', line.data_b, sequence.extras)
            file.write(sequence_a)
            file.write(sequence_b)
