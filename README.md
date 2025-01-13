# Mini Self-Assigned Project: CLI Calculator

This project is a command-line calculator written in C++. It was inspired by a line in Ray Lischner's book, *Exploring C++*, which reads:

> "If someone disputes the fact that the `main` function could also be `void`, refer the skeptic to section 3.6.1 of the C++ standard."

This statement piqued my curiosity about the intricacies of the C++ standard and led me to create this simple, yet powerful, calculator program.

## Features

- Handles basic arithmetic operations: addition (`+`), subtraction (`-`), multiplication (`*`), division (`/`), and exponentiation (`^`).
- Tokenizes input expressions for evaluation.
- Provides clear error handling for invalid characters and division by zero.

## File Structure

```
.
├── calc.cpp      # Source code for the calculator
└── install-calc.sh # Script to compile and install the calculator
```

## Getting Started

### Installation

1. Clone this repository or download the files.
2. Open a terminal in the project directory.
3. Run the installation script:
   ```bash
   ./install-calc.sh
   ```
   This will compile `calc.cpp` and make the `calc` binary executable.

   **Note for Windows Users:** If you're using Git Bash or WSL, the script will work as-is. However, you may need to manually add the project directory to your PATH or run the binary directly using `./calc`.

### Usage

Once installed, you can use the calculator directly from your terminal. Pass a mathematical expression as an argument:

```bash
calc "3+4*2"
```

#### Example Output:

```bash
11
```

For Windows users, if you encounter issues running `calc`, try using the full path or adding the directory to your PATH environment variable. Alternatively, run it like this:

```bash
./calc "3+4*2"
```

## Note on PATH Configuration

To use the calculator from any directory, ensure that the directory containing `calc` is in your PATH:

- **Linux/Mac:** Add the following line to your shell configuration file (e.g., `~/.bashrc`, `~/.zshrc`):

  ```bash
  export PATH="$PATH:/path/to/project"
  ```

  Then, source the file:

  ```bash
  source ~/.bashrc  # Or ~/.zshrc
  ```

- **Windows:**

  1. Open the Environment Variables settings.
  2. Add the full path to the directory containing `calc` to the `Path` variable.
  3. Open a new terminal to apply the changes.

## Future Improvements

- Add support for parentheses for more complex expressions.
- Improve error handling and user feedback.
- Extend support for additional mathematical functions (e.g., trigonometry, logarithms).

## Acknowledgments

Special thanks to Ray Lischner's *Exploring C++* for the inspiration to dive deeper into the C++ standard and take on this project.

