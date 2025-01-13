
# CLI Calculator

![calc being used in zsh terminal as demo](images/demo.gif)


A command-line calculator built in C++ that handles basic arithmetic operations through terminal arguments. Inspired by a deep dive into C++'s command-line argument handling (`argc` and `argv`), this project demonstrates how to create practical CLI tools while exploring core language concepts.

[Jump to Installation](#installation)

## The Story Behind

It all started with a single line in Ray Lischner's *Exploring C++*:

> "If someone disputes the fact that the `main` function could also be `void`, refer the skeptic to section 3.6.1 of the C++ standard."

That seemingly innocuous statement sent me tumbling down a rabbit hole of C++ intricacies. As I delved deeper into the Linux ecosystem, watching commands like `pwd`, `ls`, and `grep` dance across my terminal, I began to see the elegant simplicity of command-line tools. Take `grep -i "find this string"` – a command I'd used countless times without truly understanding its inner workings.

The magic, I discovered, lay in those mysterious parameters we often see in C++'s main function:

```cpp
int main(int argc, char* argv[])
```

Most newcomers to C++ (myself included) stick to the simpler `int main()`, intimidated by what appears to be cryptic syntax. But behind that intimidating façade lies a powerful concept: command-line arguments.

Consider a common compilation command:

```bash
g++ hello.cpp -o hello
```

Each space-separated element here becomes an argument passed to the program:
- `g++`: The program name itself (argument 0)
- `hello.cpp`: The input file
- `-o`: The output flag
- `hello`: The desired output filename

`argc` counts these arguments, while `argv` stores them in an array of strings, opening up a world of possibilities for program interaction. This understanding led me to create this calculator, transforming what seemed like a basic project into an exploration of how command-line tools actually work under the hood.

## Installation

1. Clone this Git repository:
    ```bash
    git clone git@github.com:mantejjosan/calc.git
    ```

    If you haven't set up SSH, check out this easy [step-by-step guide](https://mantejjosan.github.io/tutorials/CollaborateOnGithub/SetUpSshKey).

2. Run the installation script:
    ```bash
    bash install-calc.sh
    ```

3. Done! You can now use the calculator:
    ```bash
    calc 34+35
    ```

## Contribute

Feel free to do so ;)

## Future Improvements

- Add support for parentheses for more complex expressions.
- Improve error handling and user feedback.
- Extend support for additional mathematical functions (e.g., trigonometry, logarithms).
 
