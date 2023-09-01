from typing import NamedTuple

from itaxotools.common.types import Type


class UnphasedSequence(NamedTuple):
    id: str
    data: str


class PhasedSequence(NamedTuple):
    id: str
    data_a: str
    data_b: str


class PhaseWarning(Type):
    def __init__(self, text=''):
        self.text = text

    def __str__(self):
        return self.text

    def __eq__(self, other):
        return type(self) == type(other)

    def __hash__(self):
        return hash(type(self))


class Empty(PhaseWarning):
    def __init__(self):
        text = 'Dataset is empty!'
        super().__init__(text)


class Length(PhaseWarning):
    def __init__(self):
        text = 'Sequences are not of uniform length!'
        super().__init__(text)


class Missing(PhaseWarning):
    def __init__(self):
        text = 'Sequences contain gaps or missing nucleotides!'
        super().__init__(text)


class Duplicate(PhaseWarning):
    def __init__(self):
        text = 'Dataset contains duplicate ids!'
        super().__init__(text)


class Phased(PhaseWarning):
    def __init__(self):
        text = 'Sequences are already phased!'
        super().__init__(text)
