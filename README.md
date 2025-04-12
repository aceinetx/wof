# wof
S-Expression based programming language 
```lisp
(extern i32 printf _i8 vararg)

(struct Vec (
	(var public i32 x)
	(var public i32 y)
))

(fun Vec addVecs ((Vec a) (Vec b)) (
	(var Vec res)
	(assign res_x (+ a_x b_x))
	(assign res_y (+ a_y b_y))

	(return res)
))

(fun i32 main () (
	(var Vec a)
	(var Vec b)
	(var Vec v)

	(assign a_x 3) (assign a_y 1)
	(assign b_x 2) (assign b_y 1)
	(assign v (addVecs a b))

	(printf "%d %d\n" v_x v_y)

	(return 0)
))
```
The syntax is extremely similar to lisp, but unlike lisp, wof is compiled, statically typed and doesn't use as much dashes as lisp does

The wof compiler tries to use as less heap as possible (currently, it doesn't use it whatsoever), this guarantees that there will be no memory leaks

Wof is also very weakly typed, this means the compiler will try to cast the value to the right one when it doesn't match

If you want to contribute, I would recommend you reading [CODEBASE.md](docs/CODEBASE.md)
