# uudecode

See if we can do some file transfers over serial

WAV files created by Audacity add id3 tags. 
According to
[this](https://hydrogenaud.io/index.php?topic=43135.0)
site, the id3 tags breaks the container.
It confuses my `riff` program for sure. 
Your best bet is to convert files created with Audacity to
a proper WAV file using `ffmpeg -i in.wav out.wav`.

## Status

2021-07-26	Started.

