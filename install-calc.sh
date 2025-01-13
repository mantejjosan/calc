#!/bin/bash

# Detect OS and shell
OS=$(uname -s)
SHELL_NAME=$(basename "$SHELL")
CURRENT_DIR=$(pwd)

# Compile calc.cpp
echo "Compiling calc.cpp..."
if [[ "$OS" == "Linux" || "$OS" == "Darwin" ]]; then
  g++ calc.cpp -o calc
elif [[ "$OS" == "MINGW"* || "$OS" == "CYGWIN"* || "$OS" == "MSYS_NT"* ]]; then
  g++ calc.cpp -o calc.exe
else
  echo "Unsupported operating system: $OS"
  exit 1
fi

if [[ $? -ne 0 ]]; then
  echo "Compilation failed. Please check your calc.cpp for errors."
  exit 1
fi
echo "Compilation successful."

# Add calc to PATH
echo "Configuring PATH for $OS and $SHELL_NAME..."
if [[ "$OS" == "Linux" || "$OS" == "Darwin" || "$OS" == "Android" ]]; then
  if [[ "$SHELL_NAME" == "bash" ]]; then
    RC_FILE="$HOME/.bashrc"
    PATH_UPDATE="export PATH=\$PATH:$CURRENT_DIR"
  elif [[ "$SHELL_NAME" == "zsh" ]]; then
    RC_FILE="$HOME/.zshrc"
    PATH_UPDATE="export PATH=\$PATH:$CURRENT_DIR"
  elif [[ "$SHELL_NAME" == "fish" ]]; then
    RC_FILE="$HOME/.config/fish/config.fish"
    PATH_UPDATE="set -Ux PATH \$PATH $CURRENT_DIR"
  else
    echo "Unsupported shell: $SHELL_NAME"
    exit 1
  fi

  # Update PATH if not already added
  if ! grep -q "$CURRENT_DIR" "$RC_FILE"; then
    echo "$PATH_UPDATE" >> "$RC_FILE"
    echo "Path added to $RC_FILE"
  else
    echo "Path already exists in $RC_FILE"
  fi

  # Source the rc file (if not Fish)
  if [[ "$SHELL_NAME" != "fish" ]]; then
    echo "Sourcing $RC_FILE..."
    source "$RC_FILE"
  else
    echo "For Fish shell, changes will take effect after restarting the terminal."
  fi

elif [[ "$OS" == "MINGW"* || "$OS" == "CYGWIN"* || "$OS" == "MSYS_NT"* ]]; then
  WINDOWS_PATH=$(cygpath -w "$CURRENT_DIR")
  export PATH="$PATH:$WINDOWS_PATH"
  echo "Path updated. You may need to add $WINDOWS_PATH to your system's environment variables for persistence."
else
  echo "Unsupported operating system: $OS"
  exit 1
fi

# Print installation success message
echo "calc installed successfully!"
echo "You can now run it by typing 'calc' or 'calc.exe' depending on your platform."

