from __future__ import annotations

from itaxotools.taxi2.sequences import Sequences

from .types import PhaseWarning


def scan_sequences(sequences: Sequences) -> list[Warning]:
    sequences = iter(sequences)
    try:
        first = next(sequences)
    except StopIteration:
        return [PhaseWarning.Empty()]
    length = len(first.seq)
    has_missing = _scan_missing(first.seq)
    uniform = True
    for sequence in sequences:
        if len(sequence.seq) != length:
            uniform = False
        if not has_missing:
            has_missing = _scan_missing(sequence.seq)
    warns = []
    if not uniform:
        warns.append(PhaseWarning.Length())
    if has_missing:
        warns.append(PhaseWarning.Missing())
    return warns


def _scan_missing(seq: str) -> bool:
    for x in 'nN?-':
        if x in seq:
            return True
    return False
