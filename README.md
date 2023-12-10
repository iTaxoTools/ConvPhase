# ConvPhase

[![PyPI - Version](https://img.shields.io/pypi/v/itaxotools-convphase)](
    https://pypi.org/project/itaxotools-convphase)
[![PyPI - Python Version](https://img.shields.io/pypi/pyversions/itaxotools-convphase)](
    https://pypi.org/project/itaxotools-convphase)
[![PyPI - License](https://img.shields.io/pypi/l/itaxotools-convphase)](
    https://pypi.org/project/itaxotools-convphase)
[![GitHub - Tests](https://img.shields.io/github/actions/workflow/status/iTaxoTools/ConvPhase/test.yml?label=tests)](
    https://github.com/iTaxoTools/ConvPhase/actions/workflows/test.yml)

A convenient phase program.

The purpose of this program is to combine [PHASE](https://github.com/stephens999/phase) and [SeqPHASE](https://github.com/eeg-ebe/SeqPHASE) into a single python package, while making some crucial configuration options available.

Unfortunately, internal communication between *PHASE* and *SeqPHASE* is done through Fasta files. However, an iterative protocol was implemented to parse and write common Python datatypes. It is also possible to work with TSV and Fasta variants (MolD, HapView) by leveraging [Taxi2](https://github.com/iTaxoTools/TaxI2).

[*You may also be interested in the GUI*](https://github.com/iTaxoTools/ConvPhaseGui)

## Installation

ConvPhase is available on PyPI. You can install it through `pip`:

```
pip install itaxotools-convphase
```

## Usage

The package comes with a command-line tool:

```
convphase examples/simple.fas ambiguous.fas
convphase examples/simple.tsv phased.tsv -p 0.4 -q 0.4
```

The program will replicate the input file format for the output file.<br>
Allele identifiers are appended to the individual identifiers.

The following options from *PHASE* are available:
- phase threshold (-p)
- allele threshold (-q)
- number of iterations (-n)
- thinning interval (-t)
- burn-in (-b)

For examples on using the Python API, look at the [scripts](./scripts/) folder.

## Dependencies

Building from source requires:
- C++ compiler ([GCC](https://gcc.gnu.org/), [msvc](https://visualstudio.microsoft.com/vs/features/cplusplus/))
- [haxe](https://haxe.org/) & [hxcpp](https://lib.haxe.org/p/hxcpp/)

*PHASE* and *SeqPHASE* are included as git submodules.<br>
*Taxi2* is included as a setuptools dependency.

## Citations

*Stephens, M., Smith, N., and Donnelly, P. (2001). A new statistical method for haplotype reconstruction from population data. American Journal of Human Genetics, 68, 978--989.*

*Stephens, M., and Donnelly, P. (2003). A comparison of Bayesian methods for haplotype reconstruction from population genotype data. American Journal of Human Genetics, 73:1162-1169.*

*Flot, J.F. (2010) seqphase: a web tool for interconverting phase input/output files and fasta sequence alignments. Mol. Ecol. Resour., 10, 162–166.*
