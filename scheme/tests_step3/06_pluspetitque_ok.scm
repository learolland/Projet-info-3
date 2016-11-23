; TEST_RETURN_CODE=PASS
; TEST_COMMENT= vérifie que le "plus petit que" fonctionne

(< 2 3)
(< -15 -2)
(< -2 -15)
(< 23 -12384)
(define a (< 3 5))
a
(define b (< 3 1))
b
