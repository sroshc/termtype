CC = cc
CFLAGS = -Wall -g
OBJ = driver.o word.o term.o worddb.o
TARGET = termtype

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) -l sqlite3

driver.o: driver.c lib/wordlib.h
	$(CC) $(CFLAGS) -c driver.c

word.o: lib/word.c lib/wordlib.h
	$(CC) $(CFLAGS) -c lib/word.c

term.o: lib/term.c lib/termlib.h
	$(CC) $(CFLAGS) -c lib/term.c

worddb.o: lib/worddb.c lib/worddblib.h
	$(CC) $(CFLAGS) -c lib/worddb.c

clean:
	rm -f $(OBJ) $(TARGET)
