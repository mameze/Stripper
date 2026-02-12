# Version Management Quick Reference

## Understanding Semantic Versioning

```
       MAJOR . MINOR . PATCH
         2  .  11   .  10

MAJOR (2):  Breaking changes, incompatible API
MINOR (11): New features, backward compatible  
PATCH (10): Bug fixes, backward compatible
```

## Real-World Examples

### Patch (Bug Fix)
```
1.0.0 → 1.0.1
- Fixed: Comments in strings were being removed
- Fixed: Crash on empty files
```

### Minor (New Feature)
```
1.0.0 → 1.1.0
- Added: Support for Python comments
- Added: --preserve-copyright flag
```

### Major (Breaking Change)
```
1.x.x → 2.0.0
- Changed: Output format (incompatible with v1)
- Removed: Deprecated -o flag
- Changed: Default behavior now in-place
```

## How Your Friends Do It

### 1. They Define Version in Code
```c
#define VERSION "1.11.1"  // In C/C++
```
```python
__version__ = "1.11.1"  # In Python
```
```javascript
"version": "1.11.1"  // In package.json
```

### 2. They Use Git Tags
```bash
# Tag a release
git tag -a v1.11.1 -m "Release version 1.11.1"
git push origin v1.11.1

# List all versions
git tag -l

# Checkout a specific version
git checkout v1.11.1
```

### 3. They Use Build Systems

**Makefile:**
```make
VERSION = 1.11.1
CFLAGS = -DVERSION=\"$(VERSION)\"
```

**CMake:**
```cmake
project(MyProject VERSION 1.11.1)
```

**npm (JavaScript):**
```bash
npm version patch  # 1.0.0 → 1.0.1
npm version minor  # 1.0.0 → 1.1.0
npm version major  # 1.0.0 → 2.0.0
```

### 4. They Automate It

**Using scripts:**
```bash
./bump_version.sh minor
```

**Using tools:**
- **npm**: `npm version`
- **Python**: `bump2version`
- **Go**: `git tag`
- **Rust**: `cargo bump`

## For Your Comment Stripper

### Quick Release Workflow

```bash
# 1. Make your changes
vim comment_stripper.c

# 2. Test
make clean && make && make test

# 3. Bump version (choose one)
./bump_version.sh patch   # Bug fix:  1.0.0 → 1.0.1
./bump_version.sh minor   # Feature:  1.0.0 → 1.1.0
./bump_version.sh major   # Breaking: 1.0.0 → 2.0.0

# 4. Commit and tag
git add .
git commit -m "Add new feature"
git tag -a v1.1.0 -m "Release v1.1.0"

# 5. Push
git push && git push --tags

# 6. Build release
make clean && make
```

### When to Bump What

**PATCH (1.0.x):**
- Fixed a bug
- Fixed a typo
- Performance improvement (no API change)
- Documentation fixes

**MINOR (1.x.0):**
- Added new command-line flag
- Added support for new file types
- Added new feature (backward compatible)
- Deprecated a feature (but still works)

**MAJOR (x.0.0):**
- Removed a command-line flag
- Changed default behavior
- Changed output format
- Removed deprecated features
- Any breaking change

## Popular Version Formats

1. **Semantic (Most Common)**: `1.11.1`
2. **Date-based**: `2024.01.15`
3. **Named releases**: `v1.11.1-beta`, `v2.0.0-rc1`
4. **Marketing**: `Windows 11`, `macOS Ventura`

## Tips

✓ **Always** update version before release
✓ **Tag** every release in git
✓ Keep a **CHANGELOG.md** file
✓ Use **annotated tags**: `git tag -a` (not `git tag`)
✓ Follow **SemVer** if building a library
✓ Put version in `--version` flag

✗ Don't skip versions (no 1.0.2 → 1.0.4)
✗ Don't reuse version numbers
✗ Don't forget to update all files

## Example Workflow in Action

```bash
# Start fresh project
echo "1.0.0" > VERSION
git tag -a v1.0.0 -m "Initial release"

# Fix a bug
vim code.c
./bump_version.sh patch  # 1.0.0 → 1.0.1
git commit -am "Fix buffer overflow"
git tag -a v1.0.1 -m "Bug fix release"

# Add feature
vim code.c
./bump_version.sh minor  # 1.0.1 → 1.1.0
git commit -am "Add JSON output"
git tag -a v1.1.0 -m "Add JSON output feature"

# Breaking change
vim code.c
./bump_version.sh major  # 1.1.0 → 2.0.0
git commit -am "Change API: removed deprecated flags"
git tag -a v2.0.0 -m "Major API changes"

# Push everything
git push && git push --tags
```

## GitHub Releases

After tagging, create a release on GitHub:
1. Go to your repo → Releases → "Draft a new release"
2. Choose your tag (v1.1.0)
3. Add release notes
4. Upload compiled binaries
5. Publish!

---

**Remember:** Versioning is communication with your users!
