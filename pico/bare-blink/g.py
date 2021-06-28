# just fooling around with python for gdb

import gdb

RESETS_BASE = 0x4000c000
RESETS_RESET = RESETS_BASE + 0xcc

def boot2():
    print("hello")

def resets():
    #print(RESETS_RESET.derefrence())
    print(*RESETS_RESET)


resets()    
print("Finished")
