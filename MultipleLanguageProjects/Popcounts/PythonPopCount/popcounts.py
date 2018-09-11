import time, random

def popcount_1_data(x):
    c = 0
    for i in range(0, 64):
        c += x&1
        x >>= 1
    return c


def popcount64c(x):
    x -= (x >> 1) & 0x5555555555555555
    x = (x & 0x3333333333333333) + ((x >> 2) & 0x3333333333333333)
    x = (x + (x >> 4)) & 0x0f0f0f0f0f0f0f0f
    return (x * 0x0101010101010101) >> 56

def builtInPop(x):
    return bin(x).count('1')

def getRandom():
    return long(random.randint(1, 10000))

for _ in range(0, 10000):
    rand = getRandom()
    popcount64c(rand)
    popcount_1_data(rand)
    builtInPop(rand)
random = getRandom()

start1 = time.clock()
popcount_1_data(random)
end1 = time.clock()
print "Time for popcount 1 data: ", "%.20f" % (end1 - start1)

start2 = time.clock()
popcount64c(random)
end2 = time.clock()
print "Time for popcount 64c ", "%.20f" % (end2 - start2)

start3 = time.clock()
builtInPop(random)
end3 = time.clock()
print "Time for built in popcount ", "%.20f" % (end3 - start3)
