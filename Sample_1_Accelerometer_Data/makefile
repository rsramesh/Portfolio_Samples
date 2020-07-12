OBJS = read_bin.o
CC = gcc
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)
EXE = read_bin.exe
SRC = read_bin.c

all: $(EXE)

$(EXE): $(OBJS)
	 $(CC) -o $(EXE) $(OBJS)

$(OBJS): read_bin.c
	 $(CC) $(CFLAGS) $(SRC)

clean:
	 rm $(OBJS) $(EXE)
