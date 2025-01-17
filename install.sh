#!/bin/bash

INSTALL_DIR=${1:-$HOME/.local/bin}  # Default to $HOME/.local/bin if no directory is passed

# Ensure the install directory exists
mkdir -p "$INSTALL_DIR"

# Clone the repository if not already present
REPO_URL="https://github.com/mantejjosan/calc.git"
CURRENT_DIR=$(pwd)
PROJECT_DIR="$CURRENT_DIR/calc"

if [[ ! -d "$PROJECT_DIR" ]]; then
    echo "Cloning repository from $REPO_URL..."
    git clone "$REPO_URL" "$PROJECT_DIR"
else
    echo "Repository already exists at $PROJECT_DIR. Pulling latest changes..."
    cd "$PROJECT_DIR" && git pull
fi

# Create the build directory
BUILD_DIR="$PROJECT_DIR/build"
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# Run CMake and make to build the project
echo "Building the project..."
cmake ..
make

# Install binary
echo "Installing binary to $INSTALL_DIR..."
cp calc "$INSTALL_DIR"
chmod +x "$INSTALL_DIR/calc"

# Add to PATH if not already included
SHELL_RC="$HOME/.${SHELL##*/}rc"
if ! grep -q "$INSTALL_DIR" "$SHELL_RC"; then
    echo "Adding $INSTALL_DIR to PATH in $SHELL_RC"
    echo "export PATH=\$PATH:$INSTALL_DIR" >> "$SHELL_RC"
    source "$SHELL_RC"
fi

echo "Installation complete! You can now run 'calc' from anywhere."

