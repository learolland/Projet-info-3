; TEST_RETURN_CODE=PASS
; TEST_COMMENT= vérifie que define fonctionne avec lambda

( define f1 ( lambda (x) (+ x 2) ) )
(f1 3)
( define f2 ( lambda (y) (* y 2) ) )
(f2 5)
( define f3 ( lambda (a b) (* a b) ) )
(f3 5 8)
( define f4 ( lambda (z) (- (+ z 2) 3 2) )
(f4 5)
(procedure? f1)
(procedure? f2)
(procedure? f3)
(procedure? f4)
(define h (lambda (x y z)  (* (+ x y) z) ) )
(h 1 2 3)
( define carre ( lambda (y) (* y y) ) )
(carre 3)