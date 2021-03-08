CXXFLAGS = -ggdb -std=c++17

newforth : newforth.cc
	g++ $(CXXFLAGS) $^ -o $@

#lex.yy.c : scanner.l
#	flex scanner.l

clean :
	rm -f newforth heap.bin
