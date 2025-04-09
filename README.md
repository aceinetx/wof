# wof
S-Expression based programming language 
```lisp
(extern i32 printf ptr vararg)

(fun i32 add ((i32 a) (i32 b)) (
	(return (+ a b))
))

(fun i32 main () (
	(printf "One & one make %d!\n" (add 1 1))

	(return 0)
))
```
The syntax is extremely similar to lisp, but unlike lisp, wof is compiled, statically typed and doesn't use as much dashes as lisp does

The wof compiler tries to use as less heap as possible (currently, it doesn't use it whatsoever), this guarantees that there will be no memory leaks

Wof is also very weakly typed, this means the compiler will try to cast the value to the right one when it doesn't match
