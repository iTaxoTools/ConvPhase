from itaxotools.convphase.convphase import _iterator_test

x = (str(i) for i in range(10))
y = _iterator_test(x)
for i in range(10):
    z = next(y)
    print(z)

del y
print('Done')
