CXXFLAGS = -g

BINS = uutest riff wav10 ptx


ptx : ptx.cc
	g++ $(CXXFLAGS) $< -o $@

wav10 : wav10.cc
	g++ $(CXXFLAGS) $< -o $@

riff : riff.cc
	g++ $(CXXFLAGS) $< -o $@

uutest : uutest.cc lorem.uue
	g++ uutest.cc -o uutest

lorem.uue : lorem.txt
	uuencode lorem < lorem.txt > lorem.uue


clean :
	rm -f $(BINS)
