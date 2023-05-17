from __future__ import annotations

from pathlib import Path
from typing import NamedTuple

import pytest

from itaxotools.convphase import iter_phase
from itaxotools.convphase.types import PhasedSequence, UnphasedSequence


class ConvPhaseTest(NamedTuple):
    unphased_fixture: list[tuple[str, str]]
    phased_fixture: list[tuple[str, str, str]]

    @property
    def unphased(self) -> list[tuple[str, str]]:
        return self.unphased_fixture()

    @property
    def phased(self) -> list[tuple[str, str]]:
        return self.phased_fixture()

    def validate(self):
        results = iter_phase(self.unphased)
        results = list(results)
        for result, fixed in zip(results, self.phased):
            assert result == fixed


def unphased_dummy() -> list[tuple[str, str]]:
    return [
        UnphasedSequence('id1', 'AAA'),
        UnphasedSequence('id2', 'GGG'),
    ]


def phased_dummy() -> list[tuple[str, str, str]]:
    return [
        PhasedSequence('id1', 'AAA', 'AAA'),
        PhasedSequence('id2', 'GGG', 'GGG'),
    ]


convphase_tests = [
    ConvPhaseTest(unphased_dummy, phased_dummy),
]


convphase_tests_failing = [
]


@pytest.mark.parametrize('test', convphase_tests)
def test_convphase(test: AlignTest) -> None:
    test.validate()


@pytest.mark.xfail
@pytest.mark.parametrize('test', convphase_tests_failing)
def test_convphase_failing(test: AlignTest) -> None:
    test.validate()
