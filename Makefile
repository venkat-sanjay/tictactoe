# Makefile for tictactoe project with Kosaraju-Sharir algorithm

CC = gcc
CFLAGS = -Wall -Wextra -std=c99

# Default target
all: tictactoe kosaraju kosaraju_examples

# Tic-tac-toe game
tictactoe: main.c
	$(CC) $(CFLAGS) -o tictactoe main.c

# Kosaraju algorithm - basic implementation
kosaraju: kosaraju.c
	$(CC) $(CFLAGS) -o kosaraju kosaraju.c

# Kosaraju algorithm - examples
kosaraju_examples: kosaraju_examples.c
	$(CC) $(CFLAGS) -o kosaraju_examples kosaraju_examples.c

# Run all programs
run-all: all
	@echo "=== Running Tic-Tac-Toe (press Ctrl+C to exit) ==="
	@echo "Skipping interactive tic-tac-toe for automated demo"
	@echo ""
	@echo "=== Running Kosaraju Algorithm ==="
	./kosaraju
	@echo ""
	@echo "=== Running Kosaraju Examples ==="
	./kosaraju_examples

# Clean build artifacts
clean:
	rm -f tictactoe kosaraju kosaraju_examples

# Help
help:
	@echo "Available targets:"
	@echo "  all              - Build all programs"
	@echo "  tictactoe        - Build tic-tac-toe game"
	@echo "  kosaraju         - Build basic Kosaraju algorithm"
	@echo "  kosaraju_examples - Build Kosaraju examples"
	@echo "  run-all          - Build and run all programs"
	@echo "  clean            - Remove build artifacts"
	@echo "  help             - Show this help"

.PHONY: all run-all clean help