; TEST_RETURN_CODE=PASS
; TEST_COMMENT= vérifie que le "plus petit que" fonctionne

(> 3 2)
(> -2 -15)
(> -15 -2)
(> 5 +4 )
(> (* 3 2 12 4) (+ 3 2 3 4))
(define a (> 5 2))
a
(define b (> 1 3))
b
