CC     = gcc
CFLAGS = -Wall -Wextra -Wno-unused-parameter -Wno-sign-compare -std=gnu11
EXE    = mp3_quiz_generator
OBJ    = mp3_tester.o path.o string_array.o expand_file_size.o

.PHONY: clean cleano gdb

$(EXE): $(OBJ)
	$(CC) $(OBJ) -o $(EXE) $(CFLAGS)


mp3_tester.o: path.h string_array.h expand_file_size.h

%.o : %.c %.h
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -f $(EXE) $(OBJ)

cleano:
	rm -f $(OBJ)	

build:
	$(MAKE) clean
	$(MAKE) $(EXE)
	$(MAKE) cleano
