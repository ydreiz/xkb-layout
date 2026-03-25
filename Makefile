.PHONY: all release debug install uninstall clean

CC = gcc
CFLAGS_COMMON = -Wall -Wextra -pedantic
LDFLAGS = -lX11

PREFIX = $(HOME)/.local
BINDIR = $(PREFIX)/bin

TARGET = xkb-layout
SRC = main.c

all: debug
debug: CFLAGS = $(CFLAGS_COMMON) -ggdb -DDEBUG
debug: $(TARGET)

release: CFLAGS = $(CFLAGS_COMMON) -O3 -march=native -flto -DNDEBUG -s
release: clean $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

install: release
	@echo "Installing $(TARGET) to $(BINDIR)..."
	install -D -m 755 $(TARGET) $(BINDIR)/$(TARGET)

uninstall:
	@echo "Removing $(TARGET) from $(BINDIR)..."
	rm -f $(BINDIR)/$(TARGET)

clean:
	rm -f $(TARGET)
