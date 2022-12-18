### About

These are my implementations for the [Crafting Interpreters](https://craftinginterpreters.com/) Lox tree-walk interpreter and bytecode compiler.

### Setup

This project requires cmake and a C/C++ compiler

```
brew install cmake 
```

### Tree-Walk Interpreter

The Tree-Walk interpreter is written in C++ 17 and implements all the features of the Lox language. This example shows some object oriented example, and a recursive fibonacci example.

```
> sh lox.sh lox/example.lox

Bark
Meow
calculation took 759 milliseconds
```

### Bytecode Compiler

The bytecode compiler is single-pass and implements the Lox language up to, and including this chapter https://craftinginterpreters.com/calls-and-functions.html. This is written in C 99. It has the same recursive fibonacci example, but executes around 100 times faster. It also prints out the compiled bytecode before executing.

```
> sh clox.sh lox/example.clox

calculation took 
8.39
 milliseconds
```




