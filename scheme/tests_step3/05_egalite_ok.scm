; TEST_RETURN_CODE=PASS
; TEST_COMMENT= vérifie que la vérification d'égalité fonctionne

(= 2 2)
(= 3 3 3 3 3 3)
(= 100 100 100 100)
(= -3 -3 -3 -3)
(= 0 0 0 0)
(= +5 +5 +5 +5)
(= 3 (quotient 6 2) (+ 1 1 1) (- 5 1 1))
(= 2 3)
(= 100 100 100 3)
(define a (= 3 3))
a
(define b (= 3 4))
b
