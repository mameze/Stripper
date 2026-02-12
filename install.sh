#!/usr/bin/env bash
set -e

# -------------------------------
# comment_stripper install script
# -------------------------------

REPO_URL="https://github.com/mameze/Stripper"
BINARY_NAME="comment_stripper"
TMP_DIR=$(mktemp -d)

echo " Installing comment_stripper..."

# Clone the repo into temp directory
git clone "$REPO_URL" "$TMP_DIR"

cd "$TMP_DIR"

# Build the binary
echo " Building from source..."
make

# Install to /usr/local/bin
echo " Installing binary..."
sudo cp "$BINARY_NAME" /usr/local/bin/
sudo chmod +x /usr/local/bin/"$BINARY_NAME"

# Clean up
rm -rf "$TMP_DIR"

echo " comment_stripper installed successfully!"
echo "You can now run 'comment_stripper file.c' from anywhere."
