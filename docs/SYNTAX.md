# Syntax documentation
The syntax of wof is really similar to lisp, sometimes even the same as lisp

Wof uses S-expressions, which utilize a lot of parentheses. Like here:
```lisp
" This program calculates 2 + 2 "
(fun i32 main () (
	(return (+ 2 2))
))
```
Math expressions are a lot different, the operator goes first, and operands go after the operator. Same applies for comparison operators. Take this for example (from tests/calc.wof)
```lisp
(fun i32 performOperation ((i8 operand) (i64 left) (i64 right)) (
	(var i64 result)
	(assign result 0)

	(if (== operand '+') (
		(assign result (+ left right))	
	))
	(elif (== operand '-') (
		(assign result (- left right))	
	))
	(elif (== operand '*') (
		(assign result (* left right))	
	))
	(endif)

	(return result)
))
```
Struct syntax is what you would expect (almost). The only difference is that the access modifiers go in the expression
```lisp
(struct Vec2 (
	(var public f64 x)
	(var public f64 y)
))
```
Extern syntax is pretty straightforward too
```lisp
(extern i32 printf ptr vararg)
(extern i64 performOperation i8 i64 i64)
```