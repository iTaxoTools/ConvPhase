
"""Console entry point"""

from argparse import ArgumentParser
from pathlib import Path
from sys import stderr

from .main import main


def run():

    parser = ArgumentParser(description='Convenient Phase')

    parser.add_argument('input', type=str, help='Path to input file')
    parser.add_argument('-o', '--output', type=str, help='Path to output file')

    parser.add_argument('-n', '--number-of-iterations', type=int, default=100, help='Number of MCMC iterations')
    parser.add_argument('-t', '--thinning-interval', type=int, default=1, help='Thinning interval')
    parser.add_argument('-b', '--burn-in', type=int, default=100, help='Burn in')

    parser.add_argument('-p', '--phase-threshold', type=float, default=0.9, help='Phase threshold (phase certainty)')
    parser.add_argument('-q', '--allele-threshold', type=float, default=0.9, help='Allele threshold (genotype certainty)')

    args = parser.parse_args()

    input = args.input
    with open(args.input) as file:
        sequences = file.read()
    del args.input

    output = args.output
    del args.output

    print('', file=stderr)
    print(f'Calling convphase on {input} with parameters:', file=stderr)
    for k, v in vars(args).items():
        print(f'> {k} = {v}', file=stderr)
    results = main(sequences, **vars(args))

    if output is None:
        print()
        print(results)
        print()
        return

    with open(output, 'w') as file:
        print(f'Writing results to {output}...', file=stderr)
        file.write(results)
        file.write('\n')
