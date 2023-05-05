from itaxotools.convphase.convphase import _iterator_test

x = [
    ('id1', 'AAA'),
    ('id2', 'CCC'),
    ('id3', 'TTT'),
]

y = _iterator_test(x)

for z in y:
    print(z)

del y
print('Done')
