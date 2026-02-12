#!/bin/bash
# Version bump script for Comment Stripper
# Usage: ./bump_version.sh [major|minor|patch]

VERSION_FILE="VERSION"
C_FILE="comment_stripper.c"
MAKEFILE="Makefile"

if [ ! -f "$VERSION_FILE" ]; then
    echo "Error: VERSION file not found"
    exit 1
fi

# Read current version
CURRENT_VERSION=$(cat "$VERSION_FILE")
IFS='.' read -r MAJOR MINOR PATCH <<< "$CURRENT_VERSION"

# Determine bump type
BUMP_TYPE=${1:-patch}

case $BUMP_TYPE in
    major)
        MAJOR=$((MAJOR + 1))
        MINOR=0
        PATCH=0
        ;;
    minor)
        MINOR=$((MINOR + 1))
        PATCH=0
        ;;
    patch)
        PATCH=$((PATCH + 1))
        ;;
    *)
        echo "Usage: $0 [major|minor|patch]"
        echo "  major - Incompatible API changes (x.0.0)"
        echo "  minor - New features, backward compatible (0.x.0)"
        echo "  patch - Bug fixes, backward compatible (0.0.x)"
        exit 1
        ;;
esac

NEW_VERSION="$MAJOR.$MINOR.$PATCH"

echo "Bumping version: $CURRENT_VERSION → $NEW_VERSION ($BUMP_TYPE)"

# Update VERSION file
echo "$NEW_VERSION" > "$VERSION_FILE"
echo "✓ Updated $VERSION_FILE"

# Update C file
sed -i "s/#define VERSION \".*\"/#define VERSION \"$NEW_VERSION\"/" "$C_FILE"
echo "✓ Updated $C_FILE"

# Update Makefile
sed -i "s/^VERSION_MAJOR = .*/VERSION_MAJOR = $MAJOR/" "$MAKEFILE"
sed -i "s/^VERSION_MINOR = .*/VERSION_MINOR = $MINOR/" "$MAKEFILE"
sed -i "s/^VERSION_PATCH = .*/VERSION_PATCH = $PATCH/" "$MAKEFILE"
echo "✓ Updated $MAKEFILE"

echo ""
echo "Version bumped to $NEW_VERSION"
echo ""
echo "Next steps:"
echo "  1. Review changes: git diff"
echo "  2. Commit: git add . && git commit -m 'Bump version to $NEW_VERSION'"
echo "  3. Tag: git tag -a v$NEW_VERSION -m 'Release v$NEW_VERSION'"
echo "  4. Push: git push && git push --tags"
