# Compiler Construction Course Repository

Welcome to my Compiler Construction coursework repository. This repository contains all my lab exercises, assignments, standalone implementations, and exam preparation materials focused on compiler design, lexical analysis, parsing, and automata theory.

## 🗂️ Repository Structure

Here is a breakdown of how the repository is organized:

### 📝 Assignments
Major coursework assignments focusing on core compiler components.
* **`Assignment-2/`**
  * `Part-1-Lexical-analysis/`: Implementation of a lexical analyzer (scanner).
  * `Part-2-Recursive-Descent-Parser/`: Implementation of a top-down recursive descent parser.
* **`Assignments/`**
  * `Auto-Indentation/`: A script/tool to auto-indent source code based on language grammar.
  * `NFA-to-DFA/`: Algorithm implementation to convert a Nondeterministic Finite Automaton (NFA) to a Deterministic Finite Automaton (DFA), including `include/` headers and `utils/` helper functions.

### ⚙️ Lexers & Automata
Standalone projects demonstrating different ways to build tokenizers and state machines.
* **`Ad-Hoc-Lexer/`**: A custom-built lexical analyzer written from scratch without generator tools.
* **`Flex-Lexer/`**: A lexical analyzer generated using the Flex tool.
* **`DFA-Implementation/`**: A programmatic implementation of a Deterministic Finite Automaton.

### 🛠️ Parsing Tools
* **`Bison+Lex/`**: Combined projects using Flex (for lexical analysis) and Bison (for syntax analysis/parsing) to build full front-end compiler pipelines.

### 🔬 Labs
Weekly laboratory exercises and tasks covering progressive concepts in compiler design.
* `Lab-02` through `Lab-14`: Individual lab sessions covering topics ranging from regular expressions to syntax-directed translation and intermediate code generation. 

### 📚 Exams & Preparation
* **`MSE/`**: Mid-Semester Exam materials and related code.
* **`Lab-Exam-Prep/`**: Practice tasks and mock implementations for lab exams, including `Day1` and `Lab-10` focus areas.
* **`.vscode/`**: Local workspace configuration for Visual Studio Code (debugging setups, tasks).

---

## 🚀 Getting Started

### Prerequisites
To compile and run the projects in this repository, you will generally need the following tools installed:
* **C/C++ Compiler**: GCC (`g++`) or Clang.
* **Flex**: Fast Lexical Analyzer Generator.
* **Bison**: GNU parser generator.

### Common Commands
For projects using **Flex** and **Bison**, the standard compilation workflow looks like this:

1. Generate the lexer C code:
```bash
   flex lexer.l
```
2. Generate the parser C code:
```
bison -d parser.y
```
3. Compile Everything together
```
gcc lex.yy.c parser.tab.c -o compiler
```

