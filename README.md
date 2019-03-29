# Beke
A little experimental language.

I'm sure the syntax is going to change, but here's a little (VERY incomplete) EBNF:

```
stmt := expr ';'
      | func-stmt
      | IDENT '=' expr ';'

expr := ARITH-EXPR
      | func-expr
      | IDENT '(' (expr ',')* expr? ')'
      
func-expr := 'fn' ':' func-boiler
func-stmt := 'fn' IDENT ':' func-boiler

func-boiler := param-list func-body
param-list := IDENT* # no commas in param list; 'fn: a b', not 'fn: a, b'
func-body := ('->' expr) | (':' stmt) | ('{' stmt* '}')
```
