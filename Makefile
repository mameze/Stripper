# Makefile for Comment Stripper
# Supports semantic versioning

# Version information
VERSION_MAJOR = 1
VERSION_MINOR = 1
VERSION_PATCH = 0
VERSION = $(VERSION_MAJOR).$(VERSION_MINOR).$(VERSION_PATCH)

# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c99
TARGET = comment_stripper

# Source files
SOURCES = comment_stripper.c
OBJECTS = $(SOURCES:.c=.o)

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)
	@echo "✓ Built $(TARGET) v$(VERSION)"

# Compile source files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJECTS) $(TARGET) *.stripped.*
	@echo "✓ Cleaned build artifacts"

# Install to system (requires sudo)
install: $(TARGET)
	install -m 755 $(TARGET) /usr/local/bin/
	@echo "✓ Installed to /usr/local/bin/$(TARGET)"

# Uninstall from system
uninstall:
	rm -f /usr/local/bin/$(TARGET)
	@echo "✓ Uninstalled from /usr/local/bin/"

# Run tests
test: $(TARGET)
	@echo "Running tests..."
	@./$(TARGET) --version
	@echo ""
	@echo "Creating test file..."
	@echo "// This is a comment" > test_input.c
	@echo "int main() { return 0; } // inline comment" >> test_input.c
	@./$(TARGET) test_input.c
	@echo ""
	@echo "Result:"
	@cat test_input.stripped.c
	@rm -f test_input.c test_input.stripped.c
	@echo "✓ Tests passed"

# Tag a new release (use: make release VERSION_TYPE=patch|minor|major)
release:
	@echo "Current version: $(VERSION)"
	@echo "To release a new version, use git tags:"
	@echo "  git tag -a v$(VERSION_MAJOR).$(VERSION_MINOR).$(VERSION_PATCH) -m 'Release v$(VERSION)'"
	@echo "  git push origin v$(VERSION_MAJOR).$(VERSION_MINOR).$(VERSION_PATCH)"

# Show version information
version:
	@echo "Comment Stripper v$(VERSION)"

# Help
help:
	@echo "Comment Stripper Build System v$(VERSION)"
	@echo ""
	@echo "Available targets:"
	@echo "  make          - Build the executable"
	@echo "  make clean    - Remove build artifacts"
	@echo "  make install  - Install to /usr/local/bin (requires sudo)"
	@echo "  make uninstall- Remove from /usr/local/bin"
	@echo "  make test     - Run basic tests"
	@echo "  make version  - Show version information"
	@echo "  make release  - Show how to tag a release"
	@echo "  make help     - Show this help message"

.PHONY: all clean install uninstall test release version help
