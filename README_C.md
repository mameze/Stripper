# Comment Stripper - C Implementation

A fast, lightweight command-line tool written in C to remove comments from C/C++ source files.

**Current Version:** 1.0.0

## Features

✓ Removes single-line (`//`) and multi-line (`/* */`) comments  
✓ Preserves string literals and character constants  
✓ Fast and memory-efficient (written in pure C)  
✓ In-place editing support  
✓ Semantic versioning built-in  
✓ No dependencies (just standard C library)  

## Quick Start

### Build

```bash
make
```

### Run

```bash
./comment_stripper input.c output.c
```

## Installation

### From source

```bash
make
sudo make install
```

This installs to `/usr/local/bin/comment_stripper`

### Uninstall

```bash
sudo make uninstall
```

## Usage

### Basic usage

```bash
# Creates input.stripped.c
./comment_stripper input.c

# Specify output file
./comment_stripper input.c output.c

# Modify file in place (⚠️ overwrites original!)
./comment_stripper -i input.c
```

### Options

```
-h, --help       Show help message
-v, --version    Show version information
-i, --in-place   Modify file in place
```

## Example

**Before (test.c):**
```c
// This is a comment
#include <stdio.h>

/* Multi-line
   comment */
int main() {
    printf("Hello"); // inline comment
    char *s = "// not a comment";
    return 0;
}
```

**After (test.stripped.c):**
```c

#include <stdio.h>

 
int main() {
    printf("Hello"); 
    char *s = "// not a comment";
    return 0;
}
```

## Versioning

This project uses **Semantic Versioning** (SemVer):

```
MAJOR.MINOR.PATCH
  │     │     │
  │     │     └─ Bug fixes (1.0.1 → 1.0.2)
  │     └─────── New features, backward compatible (1.0.0 → 1.1.0)
  └───────────── Breaking changes (1.0.0 → 2.0.0)
```

### How to Release a New Version

#### Automatic (Recommended)

Use the version bump script:

```bash
# Patch release (1.0.0 → 1.0.1) - bug fixes
./bump_version.sh patch

# Minor release (1.0.0 → 1.1.0) - new features
./bump_version.sh minor

# Major release (1.0.0 → 2.0.0) - breaking changes
./bump_version.sh major
```

The script automatically:
1. Updates `VERSION` file
2. Updates `#define VERSION` in `comment_stripper.c`
3. Updates version numbers in `Makefile`
4. Shows next steps for git commit/tag

#### Manual

1. **Update version in 3 places:**
   - `VERSION` file: `1.1.0`
   - `comment_stripper.c`: `#define VERSION "1.1.0"`
   - `Makefile`: `VERSION_MAJOR`, `VERSION_MINOR`, `VERSION_PATCH`

2. **Commit and tag:**
   ```bash
   git add .
   git commit -m "Bump version to 1.1.0"
   git tag -a v1.1.0 -m "Release v1.1.0"
   git push && git push --tags
   ```

3. **Build release binary:**
   ```bash
   make clean
   make
   ```

### Version History

- **v1.0.0** (2026-01-27) - Initial release
  - Single-line and multi-line comment removal
  - String literal preservation
  - In-place editing support

## Build System

### Makefile targets

```bash
make           # Build the executable
make clean     # Remove build artifacts
make install   # Install to /usr/local/bin (requires sudo)
make uninstall # Remove from /usr/local/bin
make test      # Run basic tests
make version   # Show current version
make help      # Show all available targets
```

## Development

### Project Structure

```
.
├── comment_stripper.c   # Main source code
├── Makefile            # Build system with versioning
├── VERSION             # Version tracking file
├── bump_version.sh     # Automatic version bump script
└── README.md           # This file
```

### Adding Features

When adding features, consider:
- Does it break existing behavior? → **Major version bump**
- Is it a new feature? → **Minor version bump**
- Is it just a bug fix? → **Patch version bump**

### Testing

```bash
make test
```

Or manually:
```bash
./comment_stripper test.c
cat test.stripped.c
```

## Technical Details

### State Machine Parser

The tool uses a finite state machine with 5 states:
- `STATE_NORMAL` - Regular code
- `STATE_SINGLE_LINE_COMMENT` - Inside `//` comment
- `STATE_MULTI_LINE_COMMENT` - Inside `/* */` comment
- `STATE_STRING` - Inside string literal `"..."`
- `STATE_CHAR` - Inside character literal `'...'`

This ensures comments inside strings aren't removed.

### Limitations

- Currently supports C/C++ style comments only
- Does not preserve original formatting perfectly
- Edge case: escaped quotes in strings might need special handling

## Future Roadmap

- [ ] v1.1.0: Support for Python, JavaScript comments
- [ ] v1.2.0: Preserve original whitespace
- [ ] v1.3.0: Keep copyright headers (with flag)
- [ ] v2.0.0: Complete rewrite with AST parsing

## Contributing

When contributing:
1. Follow the versioning guidelines
2. Update the CHANGELOG
3. Test thoroughly with `make test`
4. Submit PR with version bump if needed

## License

MIT License - Free to use, modify, and distribute!

## Author

Created as a learning project for C programming and version management.

---

**Version:** 1.0.0  
**Last Updated:** 2026-01-27
