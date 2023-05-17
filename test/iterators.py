from sys import argv
from pathlib import Path

from itaxotools.convphase.convphase import _iterator_test
from itaxotools.taxi2.sequences import SequenceHandler

input_path = Path(argv[1])

with SequenceHandler.Fasta(input_path) as file:
    x = [(a.id, a.seq) for a in file]

for z in x:
    print(z)

print('#'*50)
y = _iterator_test(x)

for z in y:
    print(z)

# del y
print('Done')
