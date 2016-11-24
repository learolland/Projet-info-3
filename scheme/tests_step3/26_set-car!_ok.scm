;TEST_RETURN_CODE=PASS
; TEST_COMMENT= vérifie que le set-car! fonctionne

(define x (list 1 2 3))
x
(set-car! x 2)
x
(set-car! y 1)
