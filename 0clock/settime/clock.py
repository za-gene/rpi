#!/usr/bin/env python3

#import getopt, sys
import argparse
from datetime import datetime, timezone

import SDL_DS3231

ds = SDL_DS3231.SDL_DS3231()

def read_ds():
    print("DS: ", ds.read_str());
    now = datetime.now(tz=timezone.utc)
    print("Now: ", now)
    return now

def full():
    ds_now = read_ds()
    ds.write_datetime(ds_now)
    print("DS: ", ds.read_str());

#print(datetime.fromtimestamp(now, tz=timezone.utc).isoformat())

if __name__ == "__main__":
    #argv = sys.argv[1:]
    #opts, args = getopt.getopt(argv, 'hr')
    parser = argparse.ArgumentParser(description = 'Set DS3231 to current UTC time')
    parser.add_argument('-r', action="store_true", help= 'read 3231, then exit')
    args = parser.parse_args()
    #print(args)
    if(args.r):
        read_ds()
        exit(1)
    full()

    #print(f"Options tuple is {opts}")

