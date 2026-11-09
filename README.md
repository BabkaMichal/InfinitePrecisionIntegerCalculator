# Infinite Precision Integer Calculator

An arbitrary-precision integer calculator implemented in **ANSI C**.

The project implements its own integer representation and arithmetic operations, allowing calculations with integers that exceed the limits of standard C data types.

## Features

- **Arbitrary-precision integers** – supports integers of virtually unlimited size, limited by available memory.
- **Arithmetic operations:**
  - Addition
  - Subtraction
  - Multiplication
  - Division
  - Modulo
  - Exponentiation
  - Negation
- **Multiple number systems:**
  - Binary (base 2)
  - Decimal (base 10)
  - Hexadecimal (base 16)
- **Input and output conversion** – supports reading and displaying numbers in binary, decimal, and hexadecimal formats.
- **Expression validation** – validates input expressions before evaluation to prevent invalid expressions from reaching the calculation logic.
- **Sign handling** – positive and negative signs are stored separately from the numerical representation.

## Internal Representation

Numbers are represented using an array of 16-bit blocks (*limbs*).

Each block stores a value in the range of 0 to 65,535, using a base of \(2^{16}\).

The complete number is represented as:

\[
N = a_0 + a_1 \cdot 2^{16} + a_2 \cdot 2^{32} + \ldots
\]

Blocks are stored from the least significant to the most significant.

The sign is maintained separately from the number's magnitude.

This custom representation allows arithmetic operations on integers much larger than those supported by built-in C integer types.

## Expression Validation

Before evaluating an expression, the calculator checks its syntax and validity.

Invalid expressions are rejected before any arithmetic operations are performed.

## Technologies

- **Language:** ANSI C
- **Memory management:** Manual dynamic memory allocation
- **Data representation:** Custom arbitrary-precision integer implementation
- **Number systems:** Binary, decimal, and hexadecimal

## Testing

The project successfully passed a university-provided validator with execution time limits.

## Purpose

The project explores low-level number representation, dynamic memory management, arithmetic algorithms, and number-system conversions through a custom arbitrary-precision integer implementation.
