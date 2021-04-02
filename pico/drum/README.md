# Drum sample

Plays a drum sample whenever a pushbutton is pressed. You can change it to whatever sample
you want, though. Maybe you'd want to add more samples, or have it trigger from a 
sequencer. The world's you lobster when it comes to expanding on the basic idea.

The code assumes a 44100 Hz sample rate using 8 bit unsigned sample values, mono output.

It uses PWM output.

You can download a samples from the web. Use Audacity to convert the sample to
a "raw" format of the type stipulated above. 

You need to convert the raw file into a C++ file. Use the `mkdata` command that you see.
it uses the `xxd` command, which needs to be installed. You can adapt it to your needs. It
is a very simple script.

Compile the project in the "usual way". Connect a button to GPIO17. Connect a speaker to GPIO15.

If the sound isn't loud enough you can use a transistor to amplify it: connect a speaker
to either 5V or 3V3 of the Pico, and the other terminal to the collector of an NPN transistor. 
A 2222A works OK. Connect GPIO15 to the base of the transistor. Connect the emitter to ground.
It will be substantially louder then.




## News

2021-04-02 Started. Working


