notes : notes.c compile-test.c
	gcc $^ -o $@

notes.c : notes.txt
	./mknotes

clean :
	rm -f notes
