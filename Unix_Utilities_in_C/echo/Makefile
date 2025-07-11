# Makefile for echo.c (Unix echo command implementation)

CC = gcc
CFLAGS = -Wall -Wextra -O2
TARGET = echo
SRC = echo.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

