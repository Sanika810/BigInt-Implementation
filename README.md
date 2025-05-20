# BigInt-Implementation

This is a C-based command-line application that supports arithmetic operations on arbitrarily large integers (Big Integers) beyond the default data type limits. It supports addition, subtraction, and multiplication of signed large integers.

## Features

* Input of two signed big integers (positive, negative, or zero).
* Addition, Subtraction, and Multiplication operations.
* Handles edge cases like addition/subtraction with zero or multiplying with zero.
* Displays the result with correct sign formatting.

## Getting Started

### Compilation

To compile the code, use a C compiler such as `gcc`:

```bash
gcc main.c -o bigint_calculator
```

### Running the Application

```bash
./bigint_calculator
```

### Input Format

You will be prompted to enter two numbers. You can enter very large integers, including negative values.

Example:

```
Enter the first number: -123456789012345678901234567890
Enter the second number: 987654321098765432109876543210
```

## Menu Options

* `1` : Perform Addition
* `2` : Perform Subtraction
* `3` : Perform Multiplication
* `4` : Exit the Program

## Internal Design

* `BigInt` is represented as a structure with an array to store digits.
* Digits are stored in reverse order (least significant digit at index 0).
* Operations are done digit-by-digit, similar to how you would do it by hand.
* Sign handling is implemented via an enum type.

## Limitations

* Division and Modulo operations are not yet implemented.
* No support for decimal numbers (only integers).

## Future Enhancements

* Implement Division and Modulus.
* Add support for input from files.
* Improve error handling for malformed input.

## Author

This project was developed as a hands-on practice of implementing data structures and arithmetic logic in C, especially focusing on working without using pointers and arrays extensively in higher-level logic.

---

Feel free to modify, expand, or integrate this with other numeric computing tools.

Happy Calculating!
