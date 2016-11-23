; TEST_RETURN_CODE=PASS
; TEST_COMMENT= vérifie que le "plus petit que" fonctionne

(> 3 2)
(> 37827 2000 1273 12 3 1)
(> -2 -15)
(> -2 -34 -100 -234 -1294)
(> 5 +4 3 2 0 -1 -4 -6)
(> (* 3 2 12 4) (+ 3 2 3 4) (- 2 1) -3)
(> 3 +3 3 +3 3 3)
(> -2 -2)
(> 1 2 3 4 3 5)
(> -13 2938 12384)
(define a (> 5 2))
a
(define b (> 1 3))
b