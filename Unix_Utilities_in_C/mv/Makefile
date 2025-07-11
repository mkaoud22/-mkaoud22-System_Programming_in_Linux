
# Makefile for mv.c (Unix mv command implementation)

CC = gcc
CFLAGS = -Wall -Wextra -O2
TARGET = mv
SRC = mv.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

