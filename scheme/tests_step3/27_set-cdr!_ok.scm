;TEST_RETURN_CODE=PASS
; TEST_COMMENT= vérifie que le set-cdr! fonctionne

(define x (list 1 2 3))
x
(set-cdr! x 4)
x
(set-cdr! y 1)
