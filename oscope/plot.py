import matplotlib.pyplot as plt
#import wiringpi
from cffi import FFI
import timeit
from time import sleep
import time
import RPi.GPIO as G
CLK = 23
CS=24
MISO = 21

ffi = FFI()
ffi.cdef("int usleep(unsigned int);")
clib = ffi.dlopen(None)
x = clib.usleep(10)

#G.cleanup()
G.setmode(G.BOARD)
G.setup(CLK, G.OUT)
G.setup(CS, G.OUT)
G.setup(MISO, G.IN)

G.output(CLK, 1)
G.output(CS, 1)

dt = 0.1
dt=0.0
dt = 9.0e-7
dt = 1e-6
dummy = 1

misses =0
def fetch():
    global misses, dt
    dummy = 1
    x = 0
    G.output(CS, 0)
    for i in range(9):
        G.output(CLK, 1)
        #sleep(dt)
        v = G.input(MISO)
        x = x *2 + v
        #print(x)
        G.output(CLK, 0)
        for j in range(10) : dummy += 1
        #sleep(dt)
        #clib.usleep(1)
    G.output(CS, 1)
    if x != 42: misses +=1
    return x

def timing():
    tim = timeit.timeit(fetch, number=10000)
    print(tim)
    print("misses =", misses)

xs = []
ys = []
for i in range(10000):
    xs.append(time.monotonic_ns())
    ys.append(fetch())
    sleep(1e-4)

x0 = xs[0]
for i in range(10000):
    xs[i] = (xs[i] - x0)/1000
    #sleep(1e-5)

for i in range(10000):
    print(xs[i], ys[i])

def show():
    plt.plot(xs, ys)    
    plt.show()
show()


            

#while True:
#    print(fetch())
#    sleep(1)

G.cleanup()
