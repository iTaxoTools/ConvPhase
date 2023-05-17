from typing import NamedTuple


class UnphasedSequence(NamedTuple):
    id: str
    data: str


class PhasedSequence(NamedTuple):
    id: str
    data_a: str
    data_b: str
