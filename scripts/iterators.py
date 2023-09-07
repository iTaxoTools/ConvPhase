from pathlib import Path
from sys import argv

from itaxotools.taxi2.sequences import SequenceHandler

from itaxotools.convphase.phase import iter_phase, set_progress_callback


def progress_callback(value, maximum, text):
    print(f'{text}: {value}/{maximum}')


set_progress_callback(progress_callback)


input_path = Path(argv[1])

with SequenceHandler.Fasta(input_path) as file:
    x = [(a.id, a.seq) for a in file]

for z in x:
    print(z)

print('#' * 50)
y = iter_phase(x, [])

for z in y:
    print(z)

# del y
print('Done')
