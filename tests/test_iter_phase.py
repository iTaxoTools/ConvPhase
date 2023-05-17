from __future__ import annotations

from pathlib import Path
from typing import NamedTuple

import pytest

from itaxotools.convphase import iter_phase
from itaxotools.convphase.types import PhasedSequence, UnphasedSequence


class ConvPhaseTest(NamedTuple):
    unphased_fixture: list[tuple[str, str]]
    phased_fixture: list[tuple[str, str, str]]
    parameters: dict[str, object]

    @property
    def unphased(self) -> list[tuple[str, str]]:
        return self.unphased_fixture()

    @property
    def phased(self) -> list[tuple[str, str]]:
        return self.phased_fixture()

    def validate(self):
        results = iter_phase(self.unphased, **self.parameters)
        results = list(results)
        for result, fixed in zip(results, self.phased):
            assert result == fixed


def unphased_dummy() -> list[UnphasedSequence]:
    return [
        UnphasedSequence('id1', 'AAA'),
        UnphasedSequence('id2', 'GGG'),
    ]


def phased_dummy() -> list[PhasedSequence]:
    return [
        PhasedSequence('id1', 'AAA', 'AAA'),
        PhasedSequence('id2', 'GGG', 'GGG'),
    ]


def unphased_simple() -> list[UnphasedSequence]:
    return [
        UnphasedSequence('id1', 'ATGCATGCA'),
        UnphasedSequence('id2', 'ATGCATCCA'),
        UnphasedSequence('id3', 'ATSCATSCA'),
        UnphasedSequence('id4', 'ATTCATSCA'),
        UnphasedSequence('id5', 'ATCCATCCA'),
    ]


def phased_simple() -> list[PhasedSequence]:
    return [
        PhasedSequence('id1', 'ATGCATGCA', 'ATGCATGCA'),
        PhasedSequence('id2', 'ATGCATCCA', 'ATGCATCCA'),
        PhasedSequence('id3', 'ATSCATCCA', 'ATSCATGCA'),
        PhasedSequence('id4', 'ATTCATGCA', 'ATTCATCCA'),
        PhasedSequence('id5', 'ATCCATCCA', 'ATCCATCCA'),
    ]


def phased_simple_p_03() -> list[PhasedSequence]:
    return [
        PhasedSequence('id1', 'ATGCATGCA', 'ATGCATGCA'),
        PhasedSequence('id2', 'ATGCATCCA', 'ATGCATCCA'),
        PhasedSequence('id3', 'ATCCATCCA', 'ATGCATGCA'),
        PhasedSequence('id4', 'ATTCATGCA', 'ATTCATCCA'),
        PhasedSequence('id5', 'ATCCATCCA', 'ATCCATCCA'),
    ]


convphase_tests = [
    ConvPhaseTest(unphased_dummy, phased_dummy, {}),
    ConvPhaseTest(unphased_simple, phased_simple, {}),
    ConvPhaseTest(unphased_simple, phased_simple_p_03, dict(phase_threshold=0.3)),
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
