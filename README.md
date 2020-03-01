# language-tools

## Overview

`language-tools` is a repository of an assortment of partial compiler implementations in many different languages.

## C

### C Lexer

For C, there is an implementation of a functioning Lexer which stores the tokens in a Doubly-Linked List.

### C Parser

However, the parser is not complete at all.

### Result Struct

To make the Lexer and Parser more flexible and not fail immediately, I use a Result struct which not only stores data but also the success state.

```C
// Example of NameResult
typedef struct nameResult
{
    Data *data;
    State state;
} NameResult;
```

## TypeScript (TS)

### TS Lexer

The Lexer is a complete framework based on [Microsoft's Monaco Editor](https://github.com/Microsoft/monaco-editor).

The Lexer works by taking in an array of Lexemes, some of which are precoded in the class, and an input string which to return an array of tokens.

Lexemes are defined by a condition function `cond` to produce a token with the `lex` function. `require` is whether to include the token type in the final token list.

```typescript
export type Lexeme = {
    cond: (c: string) => boolean;
    lex: (src: string, i: number) => Token;
    require: boolean;
};
```

There are also precoded helper functions in the class which can be used for the Lexeme property `cond`.

The class is fully customizable by extending the class and overwriting Lexemes and Functions.

### TS Parser

The Parser is incomplete.

### TS Runner

The Runner runs the Lexer and Parser.
