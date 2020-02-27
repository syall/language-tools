# Language

## Pseudocode

```python
# Program = Array of Statements
program = array of trees

# Continually parse until all Tokens are consumed
while tokens not parsed:
    success, failure = parse(statement)
    if failure:
        print failure message then fail
    add success to program

# Parse Productions at a Non-terminal
parse(production):
    for rule in production:
        success, failure = parse(rule)
        if success:
            return success, failure
    return success = false, failure = message
```

## Grammar

```bash

program ::= <stmt>*

stmt ::= <expr>
    | "def" <id> ":" <type> "=" <expr>
    | (<id>|<array-id>) "=" <expr>
    | "while" "(" <expr> ")" <block>
    | "for" "(" <stmt> ";" <expr> ";" <stmt> ")" <block>
    | "if" "(" <expr> ")" <block> ("else if" <block>)* ("else" <block>)?
    | "continue"
    | "break"
    | "return" <expr>?
    ;

id ::= [a-z][a-zA-Z0-9_]* ;

array-id ::= <id> "[" <expr> "]" ;

type ::= "string"
    | "number"
    | "bool"
    | "fn"
    | "flex"
    | "array" "of" <type>
    ;

expr ::= <expr1>
    | <expr1> "<" <expr1>
    | <expr1> ">" <expr1>
    | <expr1> "<=" <expr1>
    | <expr1> ">=" <expr1>
    | <expr1> "==" <expr1>
    | <expr1> "!=" <expr1>
    ;

expr1 ::= <expr2>
    | <expr1> "||" <expr2>
    ;

expr2 ::= <expr3>
    | <expr2> "&&" <expr3>
    ;

expr3 ::= <expr4>
    | <expr3> "+" <expr4>
    | <expr3> "-" <expr4>
    ;

expr4 ::= <expr5>
    | <expr4> "*" <expr5>
    | <expr4> "/" <expr5>
    ;

expr5 ::= <term>
    | "!" <term>
    | "-" <term>
    | "+" <term>
    ;

term ::= <bool>
    | <string>
    | <number>
    | <fn>
    | "[" <args> "]"
    | <id>
    | <id> "(" <args> ")"
    | <id> "[" <expr> "]"
    | "(" <expr> ")"
    ;

bool ::= "true" | "false" ;

string ::= "'" [:ascii:]* "'"

number ::= (-|+)? <digits> ("." <digits>)?

digits ::= [0-9]+

fn ::= "fn" "(" <param> ")" ":" <type> <block>;

param ::= (<item> ",")* <item>?

item ::= <id> ":" <type>

args :: = (<expr> ",")* <expr>?

block ::= "{" <stmt>* "}"

```
