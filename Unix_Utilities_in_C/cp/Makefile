# Makefile for cp.c (Unix cp command implementation)

CC = gcc
CFLAGS = -Wall -Wextra -O2
TARGET = cp
SRC = cp.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
	
