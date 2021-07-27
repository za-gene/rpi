
BINS = uutest riff



riff : riff.cc
	g++ $< -o $@

uutest : uutest.cc lorem.uue
	g++ uutest.cc -o uutest

lorem.uue : lorem.txt
	uuencode lorem < lorem.txt > lorem.uue


clean :
	rm -f $(BINS)
