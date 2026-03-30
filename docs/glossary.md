# Glossary

Definitions used in this manual and in **Cambridge 9618** pseudocode discussions.

**Abstract syntax tree (AST)**  
A tree data structure produced by the parser; each node is a construct (e.g. `IF`, `CALL`, binary `+`).

**Arity**  
The number of arguments a function accepts.

**Binding**  
Associating a name with storage (a variable) or code (a procedure/function).

**Boolean**  
A value that is either `TRUE` or `FALSE`.

**Built-in function**  
A function provided by the interpreter (e.g. `LENGTH`); not written by the user in Pseudocode.

**Callee / caller**  
The procedure being called, and the code that performs the `CALL`.

**Case-insensitive**  
Upper and lower spelling of identifiers and keywords are treated the same.

**Declaration**  
A statement that introduces a name and its type (`DECLARE`, `CONSTANT`, array bounds).

**Discriminant**  
The variable inspected by a `CASE OF` statement.

**Environment**  
A mapping from names to values (and related metadata); used for scope.

**Expression**  
Code that evaluates to a value (e.g. `1 + 2 * 3`, `LENGTH(s)`).

**Identifier**  
A name for a variable, procedure, function, or constant.

**Index**  
A subscript selecting an array element, e.g. `A[i]` or `G[r,c]`.

**Interpreter**  
A program that executes source code directly (here: the `pseudocode` binary), as opposed to a compiler that only emits machine code.

**Keyword**  
A reserved word such as `IF` or `INTEGER`; cannot be used as an identifier.

**Literal**  
A constant written in source (`42`, `"hi"`, `TRUE`).

**L-value**  
An expression that denotes a assignable location (variable or indexed element).

**Parameter**  
A name in a procedure/function header; receives an argument at call time.

**Pass by reference (`BYREF`)**  
The callee receives access to the caller’s variable storage.

**Pass by value (`BYVAL`)**  
The callee receives a copy of the argument value (default).

**Precedence**  
Rules that decide which operator binds tighter (e.g. `*` before `+`).

**Runtime**  
The phase when the program runs; errors found here are **runtime errors**.

**Scope**  
The region of code in which a name is visible.

**Short-circuit**  
Evaluating `AND` / `OR` without evaluating the right operand when the result is already fixed.

**Statement**  
A complete instruction (assignment, `OUTPUT`, `IF`, …); unlike an expression, it does not necessarily yield a value.

**Token**  
A minimal lexical unit from the lexer (identifier, number, `:=`-class operators, etc.).

**Truncate**  
Convert real to integer by discarding the fractional part (toward zero for `INT` here).

---

## Cambridge-specific

**9618**  
Cambridge International syllabus code for **AS & A Level Computer Science**.

**Pseudocode Guide for Teachers**  
Official document describing notation for examined components; this interpreter follows it with documented extensions.
