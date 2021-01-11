#!/usr/bin/env python3

from datetime import datetime, timezone

import SDL_DS3231

ds = SDL_DS3231.SDL_DS3231()
print(ds.read_str());
now = datetime.now(tz=timezone.utc)
print(now)
ds.write_datetime(now)
print(ds.read_str());

#print(datetime.fromtimestamp(now, tz=timezone.utc).isoformat())

