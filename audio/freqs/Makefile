notes : notes.c compile-test.c mknotes
	gcc notes.c compile-test.c -o $@

notes.c : notes.txt mknotes
	./mknotes

clean :
	rm -f notes
