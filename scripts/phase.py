#!/usr/bin/env python
from sys import argv, stdout
from pathlib import Path

from itaxotools.convphase.phase import phase_mimic_format

input_path = Path(argv[1])
output_path = Path(argv[2])

phase_mimic_format(input_path, output_path)
