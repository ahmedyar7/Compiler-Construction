# Recursive Descent Parser (C++)

## Core Idea

This program evaluates mathematical expressions using **recursive descent parsing**.

It follows this grammar:

Expression → Term (+ or - Term)
Term → Factor (\* or / Factor)
Factor → Number or (Expression)

---

## How Code Works

- **input[]** → stores expression
- **pos** → acts like a pointer
- **peek()** → looks at current character
- **get()** → reads and moves forward

---

## Execution Flow

```mermaid
flowchart TD
    A[Start] --> B[Read Input]
    B --> C["Call expression()"]
    C --> D{End reached?}
    D -- Yes --> E[Print Result]
    D -- No --> F[Error]
    E --> G[End]
    F --> G
```

---

## Expression Function

```mermaid
flowchart TD
    A["expression()"] --> B["term()"]
    B --> C{+ or - ?}
    C -- Yes --> D[get operator]
    D --> E["term()"]
    E --> F[update result]
    F --> C
    C -- No --> G[return result]
```

---

## Term Function

```mermaid
flowchart TD
    A["term()"] --> B["factor()"]
    B --> C{* or / ?}
    C -- Yes --> D[get operator]
    D --> E["factor()"]
    E --> F[update result]
    F --> C
    C -- No --> G[return result]
```

---

## Factor Function

```mermaid
flowchart TD
    A["factor()"] --> B{Is open parenthesis?}
    B -- Yes --> C["get '('"]
    C --> D["expression()"]
    D --> E{Is closing parenthesis?}
    E -- No --> F[Error]
    E -- Yes --> G["get ')'"]
    G --> H[return result]
    B -- No --> I["number()"]
```

---

## Number Function

```mermaid
flowchart TD
    A["number()"] --> B[result = 0]
    B --> C{digit?}
    C -- Yes --> D[build integer]
    D --> C
    C -- No --> E{'.'?}
    E -- Yes --> F[parse decimal]
    F --> G{digit?}
    G -- Yes --> H[add fraction]
    H --> G
    G -- No --> I[return result]
    E -- No --> I
```

---

## Explanation (Step-by-Step)

The parser works in layers:

```
Expression → Term → Factor → Number
```

- **expression()** handles `+` and `-`
- **term()** handles `*` and `/`
- **factor()** handles parentheses
- **number()** reads numeric values

---

## Example

Input: **2+3\*4**

Steps:

1. expression() → term() → factor() → 2
2. See `+`
3. term() → 3 \* 4 = 12
4. Final result = 14

---

## Key Intuition

Operator precedence works automatically because of function hierarchy:

```
expression()
    ↓
term()
    ↓
factor()
    ↓
number()
```

Multiplication is evaluated before addition because `term()` is called inside `expression()`.

---

```

---

# 🧾 Summary

- This `.md` works perfectly on GitHub
- Mermaid diagrams render automatically on GitHub
- Cleaner than HTML for documentation
- Perfect for adding to your parser project README

