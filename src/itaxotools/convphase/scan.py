from __future__ import annotations

from itaxotools.taxi2.sequences import Sequences

from .files import get_info_from_path, get_handler_from_info
from .types import PhaseWarning


def scan_sequences(sequences: Sequences) -> list[PhaseWarning]:
    sequences = iter(sequences)
    try:
        first = next(sequences)
    except StopIteration:
        return [PhaseWarning.Empty()]
    length = len(first.seq)
    has_missing = _scan_missing(first.seq)
    ids = set([first.id])
    uniform = True
    duplicates = False
    for sequence in sequences:
        if len(sequence.seq) != length:
            uniform = False
        if not has_missing:
            has_missing = _scan_missing(sequence.seq)
        if not duplicates and sequence.id in ids:
            duplicates = True
        ids.add(sequence.id)
    warns = []
    if not uniform:
        warns.append(PhaseWarning.Length())
    if has_missing:
        warns.append(PhaseWarning.Missing())
    if duplicates:
        warns.append(PhaseWarning.Duplicate())
    return warns


def scan_path(path: Path) -> list[PhaseWarning]:
    info = get_info_from_path(path)
    data = Sequences(get_handler_from_info, path, 'r', info)
    return scan_sequences(data)


def _scan_missing(seq: str) -> bool:
    for x in 'nN?-':
        if x in seq:
            return True
    return False
