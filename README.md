# wof
S-Expression based programming language 
```lisp
(fun i8 main () (
	(return 0)
))
(fun i32 add ((i32 a) (i32 b)) (
	(return (+ a b))
))
```
The syntax is extremely similar to lisp, but unlike lisp, wof is compiled, statically typed and doesn't use as much dashes as lisp does

The wof compiler tries to use as less heap as possible (currently, it doesn't use it whatsoever), this guarantees that there will be no memory leaks
