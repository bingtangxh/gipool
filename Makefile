CC ?= cc
CFLAGS ?= -O2 -Wall -Wextra -std=c11

TARGET = gipool
SRC = gipool.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean
