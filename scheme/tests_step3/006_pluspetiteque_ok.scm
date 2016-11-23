; TEST_RETURN_CODE=PASS
; TEST_COMMENT= vérifie que le "plus petit que" fonctionne

(< 2 3)
(< 2 4 27 1283 2000 37829)
(< -15 -2)
(< -1843 -213 -100 -56 -2)
(< -6 -4 -1 0 1 3 +4 5)
(< -3 (- 2 1) (+ 3 2 3 4) (* 3 2 12 4))
(< 3 3 3 3 3 3)
(< -2 -2)
(< 2 3 4 3 5)
(< 23 2938 -12384)
(define a (< 3 5))
a
(define b (< 3 1))
b