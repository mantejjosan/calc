#!/bin/bash

INSTALL_DIR=${1:-$HOME/.local/bin}  # Default to $HOME/.local/bin if no directory is passed

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
mkdir -p "$INSTALL_DIR"
cp calc "$INSTALL_DIR"

# Add to PATH if not already included
if ! grep -q "$INSTALL_DIR" "$HOME/.bashrc"; then
    echo "Adding $INSTALL_DIR to PATH in .bashrc"
    echo "export PATH=\$PATH:$INSTALL_DIR" >> "$HOME/.bashrc"
    source "$HOME/.bashrc"
fi

if ! grep -q "$INSTALL_DIR" "$HOME/.zshrc"; then
    echo "Adding $INSTALL_DIR to PATH in .zshrc"
    echo "export PATH=\$PATH:$INSTALL_DIR" >> "$HOME/.zshrc"
    source "$HOME/.zshrc"
fi

if ! grep -q "$INSTALL_DIR" "$HOME/.config/fish/config.fish"; then
    echo "Adding $INSTALL_DIR to PATH in config.fish"
    echo "set -Ux PATH \$PATH $INSTALL_DIR" >> "$HOME/.config/fish/config.fish"
    echo "For Fish shell, changes will take effect after restarting the terminal."
fi

echo "Installation complete! You can now run 'calc' from anywhere."

