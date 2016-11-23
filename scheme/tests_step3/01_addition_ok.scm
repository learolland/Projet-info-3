; TEST_RETURN_CODE=PASS
; TEST_COMMENT= vérifie que l'addition fonctionne

(+ 2)
(+ 2 3)
(+ +2 -3)
(+ 2 2 2 2 2 2 2 2 2 2)
(+ 2 -3 -34 +23 3 4)
(+ 3 4 (+ 2 3 4))
(define a (+ 2 3 4 5))
a
(+ a 32)
