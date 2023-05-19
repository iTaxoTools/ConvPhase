from __future__ import annotations

from pathlib import Path
from typing import NamedTuple

import pytest

from itaxotools.taxi2.sequences import Sequences, Sequence
from itaxotools.convphase.scan import scan_sequences
from itaxotools.convphase.types import PhaseWarning


class ScanTest(NamedTuple):
    fixture: Callable[[], Sequences]
    warns: list[PhaseWarning]

    @property
    def sequences(self) -> Sequences:
        return self.fixture()

    def validate(self):
        warns = scan_sequences(self.sequences)
        assert len(self.warns) == len(warns)
        assert set(warn() for warn in self.warns) == set(warns)


def good_sequences() -> Sequences:
    return Sequences([
        Sequence('id1', 'AAA'),
        Sequence('id2', 'GGG'),
    ])


def empty_sequences() -> Sequences:
    return Sequences([])


def missing_sequences_1() -> Sequences:
    return Sequences([
        Sequence('id1', 'AAN'),
        Sequence('id2', 'GGG'),
    ])


def missing_sequences_2() -> Sequences:
    return Sequences([
        Sequence('id1', 'AAA'),
        Sequence('id2', 'GGN'),
    ])


def missing_sequences_3() -> Sequences:
    return Sequences([
        Sequence('id1', 'AAA'),
        Sequence('id2', 'GGn'),
    ])


def missing_sequences_4() -> Sequences:
    return Sequences([
        Sequence('id1', 'AAA'),
        Sequence('id2', 'GG-'),
    ])


def missing_sequences_5() -> Sequences:
    return Sequences([
        Sequence('id1', 'AAA'),
        Sequence('id2', 'GG?'),
    ])


def non_uniform_sequences_1() -> Sequences:
    return Sequences([
        Sequence('id1', 'AA'),
        Sequence('id2', 'GGGG'),
    ])


def non_uniform_sequences_2() -> Sequences:
    return Sequences([
        Sequence('id1', 'AAAA'),
        Sequence('id2', 'GG'),
    ])


def non_uniform_and_missing_sequences() -> Sequences:
    return Sequences([
        Sequence('id1', 'AA--'),
        Sequence('id2', 'GGn'),
    ])


def duplicate_id_sequences() -> Sequences:
    return Sequences([
        Sequence('id1', 'AAA'),
        Sequence('id1', 'GGG'),
    ])




scan_tests = [
    ScanTest(good_sequences, []),
    ScanTest(empty_sequences, [PhaseWarning.Empty]),
    ScanTest(missing_sequences_1, [PhaseWarning.Missing]),
    ScanTest(missing_sequences_2, [PhaseWarning.Missing]),
    ScanTest(missing_sequences_3, [PhaseWarning.Missing]),
    ScanTest(missing_sequences_4, [PhaseWarning.Missing]),
    ScanTest(missing_sequences_5, [PhaseWarning.Missing]),
    ScanTest(non_uniform_sequences_1, [PhaseWarning.Length]),
    ScanTest(non_uniform_sequences_2, [PhaseWarning.Length]),
    ScanTest(non_uniform_and_missing_sequences,
        [PhaseWarning.Length, PhaseWarning.Missing]),
    ScanTest(duplicate_id_sequences, [PhaseWarning.Duplicate]),
]


@pytest.mark.parametrize('test', scan_tests)
def test_scan(test: ScanTest) -> None:
    test.validate()
