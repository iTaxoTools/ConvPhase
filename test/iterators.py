from itaxotools.convphase.convphase import _iterator_test

x = ['AAA', 'CCC', 'TTT']
y = _iterator_test(x)
for z in y:
    print(z)

del y
print('Done')
