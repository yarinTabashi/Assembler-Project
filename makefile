CC = gcc # GCC Compiler
CFLAGS = -ansi -Wall -pedantic -lm # Flags
EXE_DEPS = text_ana.o structs.o shared.o secondPass.o printFunct.o help_text_ana.o # Deps for exe

text_ana: $(EXE_DEPS)
	$(CC) -g $(EXE_DEPS) $(CFLAGS) -o $@
text_ana.o: text_ana.c
	$(CC) -c text_ana.c $(CFLAGS) -o $@
printFunct.o: printFunct.c
	$(CC) -c printFunct.c $(CFLAGS) -o $@
secondPass.o: secondPass.c
	$(CC) -c secondPass.c $(CFLAGS) -o $@
structs.o: structs.c 
	$(CC) -c structs.c $(CFLAGS) -o $@
shared.o: shared.c
	$(CC) -c shared.c $(CFLAGS) -o $@
help_text_ana.o: help_text_ana.c
	$(CC) -c help_text_ana.c $(CFLAGS) -o $@

# Clean Target (remove leftovers)
clean:
	rm -rf *.o