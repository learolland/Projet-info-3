; TEST_RETURN_CODE=PASS
; TEST_COMMENT= vérifie que la division fonctionne

(quotient 2)
(quotient 2 3)
(quotient 4 2)
(quotient 4 1)
(quotient 4 2 2)
(quotient 3 1 1 1 1 1 1 1 1 1)
(quotient 2 2 2 2 2 2 2 2 2 2)
(quotient 1024 2 4 1 (quotient 24 3))
(define a (quotient 48 2 3))
a
(quotient a 4)
