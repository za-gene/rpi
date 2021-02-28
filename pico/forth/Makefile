
newforth : newforth.cc lex.yy.c
	g++ -ggdb $^ -o $@

lex.yy.c : scanner.l
	flex scanner.l

clean :
	rm -f newforth heap.bin
