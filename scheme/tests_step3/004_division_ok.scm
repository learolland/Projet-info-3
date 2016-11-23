; TEST_RETURN_CODE=PASS
; TEST_COMMENT= vérifie que la division fonctionne

(/ 2)
(/ 2 3)
(/ 4 2)
(/ 4 1)
(/ 4 2 2)
(/ 3 1 1 1 1 1 1 1 1 1)
(/ 2 2 2 2 2 2 2 2 2 2)
(/ 1024 2 4 1 (/ 24 3))
(define a (/ 48 2 3))
a
(/ a 4)